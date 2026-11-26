# Build BlueJay's clustered forward renderer in nineteen steps

BlueJay builds its renderer in the 19 steps of the [render doc](https://claude.ai/artifact/JbGRFRd56ZYdSJGUESNb3E). Steps 1–11 run on SDL3 GPU, steps 12–16 port to native Vulkan 1.4, and steps 17–19 cover consoles and then VR. The core is a depth prepass, CPU-built tiles and Z-bins, one forward opaque pass that also writes a thin normal/roughness/velocity buffer, and transparents lit from the same light grid. SDL3 GPU lacks GPU-written draw counts, bindless and timestamps, so each step notes the prototype workaround and what Vulkan replaces.

## Decisions and starting point

On 22 Nov 2026 the owner confirmed the render doc's reading, and the [Roadmap](https://claude.ai/artifact/3UVpHgGb7WTsHQFMwC2ePZ) now records it. "Hybrid" means forward opaques plus a thin side buffer plus split tile/cluster binning; deferred opaques are only a fallback if VR leaves the plan. SDL3 GPU is the first backend, Vulkan 1.4 follows on PC. A minimal render graph (step 3) and GPU-driven submission (step 5) come early in the prototype.

The [repo](https://github.com/DracniaStudios/BlueJayEngine) is a C++23 CMake skeleton with SDL 3.4.20, ImGui (SDL_GPU3 backend), Tracy, EnTT, Flecs, meshoptimizer, KTX and fastgltf vendored. It has no shader compiler, Vulkan SDK or VMA yet. The old raylib/OpenGL 3.3 path (8-light limit) should be retired, not evolved.

**Prerequisites from the Roadmap:** P0 profiler, P1 SDL3 platform layer, P2 job API, P3 fixed tick ("runs identically with rendering off"), P4 offline cook. Lock contract #1 (coordinates, depth range) and #12 (photometric units, exposure, view transform) first.

## The 11-stage frame

| # | Stage | Step | SDL3 GPU form | Vulkan 1.4 change |
|---|---|---|---|---|
| 1 | CPU: simulate, extract, record | 2 | EnTT extract → frame packet → parallel recording, fixed submit order | Per-thread pools, timeline semaphores |
| 2 | GPU culling | 5 | Fixed-size indirect array, culled entries zeroed, CPU-set count | GPU-written counts |
| 3 | Depth prepass | 6a | Depth-only indirect draws, then HZB rebuild | Same |
| 4 | Light binning | 6b | CPU-built Z-bins + tile masks; clusters for transparents | GPU compaction, scalarized loops |
| 5 | Shadows | 7 | Cached CSM, stepped sun, local-light atlas | RT shadows opt-in (Ultra) |
| 6 | Forward opaque | 6c | HDR + normal/roughness/velocity side buffer | Bindless materials |
| 7 | Screen-space (first to scale down) | 8 | GTAO, SSR from side buffer | Async compute |
| 8 | Transparents and VFX | 6d | Same light grid | Same |
| 9 | Fog and sky | 7–8 | LUT sky; froxel fog on High/Ultra | Same |
| 10 | Reconstruction | 9 | MSAA+FXAA, in-house TAA/upscaler, frame-time DRS | DLSS/FSR 4/XeSS, timestamp DRS |
| 11 | Post, UI, present | 9 | PBR Neutral, UI composite, VSYNC, 2 frames in flight | Present timing, latency SDKs |

**Budget:** aim for 14–15 ms typical against the 16.67 ms ceiling, judged by 1% and 0.1% lows in the worst crowd scene. Rough envelopes: prepass 0.5–1.5 ms, binning ≤0.5 ms, opaque shading 2–5 ms, GTAO ~0.56 ms at 1080p, TAA ~1 ms. These are estimates to measure on your own hardware.

## Steps 1–5: foundation (no lights yet)

### Step 1: Instrument first
- **Build:** Tracy CPU zones, a frame-time CSV (average, 1%/0.1% lows, frames over 16.67 and 33.3 ms), a hitch detector at 2× budget, and counters for in-game shader compiles and buffer reallocations (both should read zero).
- **SDL3 GPU:** no timestamps, so per-pass GPU timing comes from RenderDoc, Nsight or RGP captures until step 12. Use SDL3 GPU debug labels so captures are readable.
- **Done when:** a scripted fly-through emits the CSV and Tracy shows frame zones.

### Step 2: Extract, prepare, record
- **Build:** one extract system, the only code that reads EnTT, writing plain arrays into a per-frame allocator. Prepare jobs run on the P2 job API. Command buffers are recorded in parallel and submitted in fixed order, which SDL3 GPU supports.
- **Present:** VSYNC, two frames in flight, and a 60 fps limiter that waits before input sampling.
- **Done when:** the game runs identically headless, the frame packet is the only ECS→renderer bridge, and present intervals hold at 16.67 ms.
- **Depends on:** a minimal EnTT/Flecs layer pulled forward from the Roadmap's P6.

### Step 3: RHI rules and a minimal render graph
- **Build:** design the RHI for Vulkan now. Six rules: parallel-recorded commands in fixed order, resource states declared through the graph, full PSO-like state per draw, one offline shader language with reflection, a resource-index layer (texture arrays now, descriptors later), fenced ring-buffer uploads. Add generational handles, immutable pipelines, load/store render passes and deferred destruction.
- **Graph:** Frostbite-style setup/compile/execute, pass culling from the final output, first/last use per resource, a graph dump. Skip aliasing, async compute and reordering. SDL3 GPU syncs itself, so the graph records barriers now and Vulkan uses them at step 12.
- **Done when:** a textured sRGB mesh renders through the graph and no `SDL_GPU*` type appears outside the backend.

### Step 4: Offline, non-blocking shaders
- **Build:** HLSL compiled offline through SDL_shadercross, a few uber-shaders, pipelines created at load on job threads, and "skip rather than block" at draw time. Materials are data in a buffer, so new gear and spells never create a pipeline (Doom Eternal ships under 500 pipelines).
- **Deferred:** a persisted pipeline cache waits for Vulkan.
- **Done when:** a new material needs no C++, hot reload works, and the compile counter stays at zero.

### Step 5: GPU culling and submission
- **Build, in order:** CPU frustum cull; mega vertex/index buffers; `MeshDesc`, `Instance` (current and previous transform) and `Material` buffers; compute cull writing indirect commands; vertex pulling; two-phase HZB occlusion.
- **SDL3 GPU:** indirect draws take a CPU-set count, so compute writes a fixed-size array and zeroes culled entries. Measure what zeroed entries cost at worst-case instance count; no source has. Use texture arrays or a virtual texture instead of bindless.
- **Deferred:** mesh shaders wait for Vulkan (Wicked Engine found them slower in real scenes).
- **Done when:** CPU submit time stays flat as instances grow 10× and HZB causes no popping.

## Step 6: the clustered forward core

The render doc gives no grid sizes, so the numbers below come from DOOM, Call of Duty and HDRP. Treat them as starting points to tune.

### 6a: Depth prepass
- **Build:** depth-only indirect draws, front to back, with alpha-test `discard` here only. The forward pass then uses depth test EQUAL with writes off. Rebuild the HZB for step 5.
- **Data:** reversed-Z float depth. Share one transform function and mark position `precise`, or EQUAL speckles appear. Write velocity for moving meshes only; rebuild static velocity from depth and camera.
- **Done when:** overdraw view shows ~1 shade per pixel and skinned characters show no speckles.

### 6b: Light binning
- **Why split:** tiles leak lights across silhouettes (Olsson: clustered 17 ms vs tiled 26 ms), but tiles plus Z-bins shade opaques well (CoD: 9.00 → 7.65 ms).
- **Build:** the CPU culls and sorts visible lights by view Z, builds a Z-bin table (16-bit min/max light ID per bin, ~32 KB), and a 256-bit mask per 8×8 tile. This needs no atomics or GPU-written counts, so it fits SDL3 GPU.
- **Transparents:** use a 3D cluster grid, e.g. 16×8×24 with slices `Z_k = near·(far/near)^(k/N)`. Cap it at a "cluster far" of roughly 300–500 m and fade lights beyond. Rebuild cluster bounds only when FOV, near/far or resolution change.
- **Also:** test spot lights as cones vs spheres. Bin decals and probes in the same grid, decals first. Keep the sun out of the grid.
- **Done when:** heatmap, slice and grid debug views exist, a CPU reference confirms lists are supersets, and binning is ≤0.5 ms in the worst town.

### 6c: Forward opaque pass
- **Build:** per fragment, find tile and bin, loop lights, evaluate metallic-roughness PBR with range-checked attenuation. Gate shadow sampling on each light's shadow index.
- **Side buffer:** HDR (R11G11B10F or RGBA16F), normal+roughness (RG16 or octahedral RGB10A2), velocity (RG16F), no albedo. Low writes velocity only. About 20 B/pixel, ~74 MB per frame at 1440p.
- **Keep MSAA working:** VR at step 19 reuses this pass, so avoid effects that need TAA to hide noise.
- **Deferred:** subgroup scalarization (72% of baseline in CoD) waits until wave intrinsics are verified.
- **Done when:** hundreds of lights render correctly with and without 4× MSAA, in 2–5 ms in the worst town.

### 6d: Transparents and spell VFX
- **Build:** sorted forward-lit glass, water and hair at full res, each using its own view Z in the grid. Spell particles go half-res with depth-aware upsample (CoD: 1.3 → 0.4 ms plus 0.3 ms upsample). Light particles per vertex or into an atlas so cost doesn't scale with resolution.
- **Fallback:** weighted blended OIT (RGBA16F + R8) where sorting fails. Mark transparents in a reactive mask for step 9.
- **Done when:** dense spell scenes light correctly and VFX cost stays flat as resolution rises.

### 6e: Gate
The worst town, with only steps 6a–6d, must hold well under 14 ms on Medium hardware (RTX 3060 class) by 1% and 0.1% lows. Budget art at ≤8–16 average and ≤64 max lights per pixel, enforced by the heatmap and a light LOD.

## Steps 7–11: effects, crowds and presets

### Step 7: Shadows and sky
- **Build:** cached sun CSM with the sun direction stepped so the cache stays valid; characters redrawn each frame. A local-light shadow atlas with static casters cached (Doom used 8k×8k). Capsule shadows far, contact shadows near, LUT sky on all presets.
- **Rule:** shadowed lights are the real cap, not light count. The atlas needs no bindless, so it works on SDL3 GPU.

| Preset | Sun | Characters | Shadowed local lights |
|---|---|---|---|
| Low | Fewest cascades | Capsule | None |
| Medium | Fewer cascades | Capsule beyond near | Few |
| High | Most cascades | Maps near, contact shadows | Budgeted |
| Ultra | Same + RT opt-in (Vulkan) | Same | More |

- **Done when:** no shadow cost spike as the sun steps or characters cross cascades.

### Step 8: GI, AO and fog
- **Build:** baked time-of-day GI or probes in a BC6H cubemap array, skipped below a gloss threshold (saved ~0.5 ms in CoD). XeGTAO compiled through shadercross, run right after the prepass so the forward pass gets same-frame AO. SSR after the forward pass, using last frame's HDR with cubemap fallback. Froxel fog on High/Ultra.
- **Presets:** AO off on Low, GTAO on Medium and up, SSR from High.
- **Done when:** turning stage 7 off removes all its cost.

### Step 9: Reconstruction, resolution and tone mapping
- **Build:** MSAA+FXAA, then an in-house TAA/upscaler. Motion vectors must include skinned characters. Honour the vendor upscaler inputs now: pre-tonemap HDR, inverted depth, un-jittered motion, jitter, exposure, reset flags. PBR Neutral tone map, UI composited after upscaling.
- **Governor:** `newScale = scale / sqrt(error)`, clamped to 50–100%, with a change threshold, minimum period and a panic drop. Without timestamps it runs on frame time. It must spot CPU-bound frames and leave resolution alone for them.
- **Done when:** no ghosting on camera whips or moving crowds, and the governor holds 16.67 ms without oscillating.

### Step 10: Crowds
- **Build:** a significance score, a ~1 ms animation budget, skinning once per frame into a shared buffer, and a LOD chain from skinned to instanced to impostor to nameplate. Per-preset character caps, spell-density limits, pooled spawns with a per-frame budget.
- **Rule:** caps never hide nearby enemies in PvP.
- **Done when:** a scripted siege holds CPU frame time on each preset's hardware.

### Step 11: Presets and acceptance
- **Build:** presets as data ("a pass list plus quality-group levels"). Worst-case crowd scenes run nightly as a fly-through that fails on 1%/0.1% regressions.
- **Done when:** every preset meets 60 fps on its hardware tier, with frame generation excluded.

## Vulkan phase: what replaces each SDL3 GPU workaround

| SDL3 GPU gap | Steps | Prototype workaround | Vulkan replacement (step) |
|---|---|---|---|
| CPU-set indirect count | 5, 6a, 7 | Fixed array, zeroed entries | `vkCmdDrawIndexedIndirectCount` (12) |
| No bindless | 5, 6b, 6c | Texture arrays, atlases | Descriptor indexing (12) |
| No timestamps | 1, 9 | Captures, frame-time governor | Timestamps, Tracy GPU zones (12) |
| VSYNC only | 2 | VSYNC + limiter | Present wait / present timing (12) |
| No pipeline cache | 4 | Load-time creation | Persisted cache (12) |
| No native handles | 9 | In-house upscaler | DLSS, FSR 4, XeSS, Reflex (13); frame gen (14) |
| No async, mesh shaders, RT | 8 | Serial compute | Async compute (15); mesh shaders (15); RT (16) |
| Wave intrinsics unverified | 6c | Plain loop | Scalarization |
| No PlayStation | 17–18 | — | D3D12 (17), Sony API (18) |

At step 12 the graph's recorded barriers become `vkCmdPipelineBarrier2` calls and the resource-index layer switches to descriptors. Use FIFO present and two frames in flight. VR (step 19) reuses the forward pass with MSAA at an 11.11 ms budget, with SSAO/SSR reduced or off.

## Biggest risks

The light-culling algorithms are proven and cheap at MMO light counts. The open risks are the unmeasured cost of zeroed indirect commands at crowd scale, and the lack of GPU timestamps until Vulkan. Measure the first with a capture as soon as step 5 lands. If it's bad, pull step 12 forward instead of adding workarounds.
