Game Engine Roadmap

Filter the map

- [Summary](#summary)
- [Category map](#category-map)
- [Framework](#framework)
  - [Structure](#structure)
    - [Layer stack](#layer-stack)
    - [Growth stages](#growth-stages)
    - [Buy-and-build split](#buy-build)
  - [Sequence](#sequence)
    - [Bring-up order](#bring-up)
    - [Sequencing disputes](#disputes)
  - [Roadmap](#roadmap)
    - [Master grid](#master-grid)
    - [Contracts](#contracts)
    - [Learning path](#learning)
- [Foundation](#foundation)
  - [Runtime](#runtime)
    - [Platform layer](#platform-layer)
    - [Memory](#memory)
    - [Concurrency](#concurrency)
  - [Core utilities](#core-utilities)
    - [Math](#math)
    - [Containers, strings, hashing and logging](#containers)
    - [Random numbers](#rng)
  - [Time and settings](#time-and-settings)
    - [Game loop and time](#loop)
    - [Configuration and CVars](#config)
  - [Files](#files)
    - [VFS and archives](#vfs)
    - [Compression](#compression)
  - [Data](#data)
    - [Asset pipeline and resources](#assets)
    - [Serialization and saves](#serialization)
- [Subsystems](#subsystems)
  - [Rendering](#rendering)
    - [Rendering core](#rendering-core)
    - [GL-class API limits](#gl-limits)
    - [Lighting and shadows](#lighting)
    - [Atmosphere and post-processing](#atmosphere)
    - [Particles and VFX](#particles)
    - [Terrain, LOD and occlusion](#terrain)
  - [Motion](#motion)
    - [Physics and collision](#physics)
    - [Animation](#animation)
  - [Media](#media)
    - [Audio](#audio)
    - [Video playback](#video)
- [Gameplay](#gameplay)
  - [Logic](#logic)
    - [Entity and object model](#entities)
    - [Events and messaging](#events)
    - [Scripting](#scripting)
  - [World](#world)
    - [Scene and world management](#scene)
    - [Cameras](#cameras)
    - [AI, perception and navigation](#ai)
    - [Procedural generation](#procgen)
- [Interface](#interface)
  - [Interaction](#interaction)
    - [Input](#input)
    - [UI](#ui)
  - [Language](#language)
    - [Text and fonts](#text)
    - [Localization](#localization)
  - [Access](#access)
    - [Accessibility](#accessibility)
- [Tools](#tools)
  - [Authoring](#authoring)
    - [Editor](#editor)
    - [Hot reload](#hot-reload)
  - [Diagnostics](#diagnostics)
    - [Debug tools and profiling](#debug)
    - [Crash reporting](#crash)
    - [Telemetry](#telemetry)
  - [Delivery](#delivery)
    - [Automated testing](#testing)
    - [Build, CI and release](#build)
- [Online](#online)
  - [Multiplayer](#multiplayer)
    - [Networking and hosting](#networking)
    - [Anti-cheat and security](#anticheat)
  - [Distribution](#distribution)
    - [Platform services](#platform-services)
    - [Patching, updates and DLC](#patching)
    - [Modding and UGC](#modding)
  - [Compliance](#compliance)
    - [Console porting and certification](#console)
    - [Legal, licensing and privacy](#legal)
- [Connections](#connections)
- [Open questions](#open-questions)
  - - [Unknowns](#unknowns)
    - [Conflicting views](#conflicts)
    - [Missing topics](#missing)
    - [Checks run](#checks)
    - [Changes made](#changes)
- [Sources](#sources)

Research artifact · updated 13 November 2026

# Game Engine Roadmap

**Research question:** What should a from-scratch game engine build, buy and lock, and in what order, across every subsystem from prototype to large-scale online?

## Summary

A from-scratch engine should grow through five stages (prototype, first shipped game, multi-platform, team and multiple titles, large-scale online), and inside the first two stages its systems come up in one dependency order, from P0 tooling to P9 online play, with a job system added only when a profiler shows CPU-bound frames.

The work splits in two: buy the leaf algorithms (physics, navigation, text shaping, crash capture) and write the seams yourself (handles and stable IDs, reflection, save and pack formats, the input action layer, the render interface, replication).

The expensive part is eighteen contracts, which are decisions that leak into assets, saves, packets, patches or other people's work, and almost all of them are cheap to lock in stages 1 and 2.

The stage gates are the report's own synthesis, because no source publishes a canonical roadmap, exit criteria or effort estimates.

This update's review found 16 open unknowns, 14 conflicting views and 27 missing topics; the largest are an undecided determinism promise, an unpicked first render backend, and no coverage of mobile and web targets, shader-compilation stutter or server orchestration.

1. S1**Prototype / first playable***Advance on a decision to ship*
2. S2**First shipped game, one platform***Advance on a second platform, console deal or second store*
3. S3**Multi-platform / console-ready***Advance on a second title, or more than a few programmers*
4. S4**Growing team, multiple titles***Advance on a persistent online world or live-service cadence*
5. S5**Large-scale / online***Final stage*

How to read this page

- Established A sentence with a linked source. Versions, prices and licenses are as observed in early November 2026.
- Inference The report's synthesis, or this review's reasoning: every stage table, “done when” gate, trigger, build-vs-buy call, pitfall list and learning list.
- Contested Sources or practitioners disagree. These are collected under Sequencing disputes and under Open questions.

- Stages are labeled S1 to S5. Bring-up phases inside S1 and S2 are labeled P0 to P10.
- Console requirements beyond Microsoft's public Xbox Requirements are under NDA (non-disclosure agreement) and are not covered.
- Evidence, stage tables and guidance are carried over word for word from the original report. The “What it is” and “Why it matters” lines, the Connections and the Open questions were written for this update.
- Dotted terms show a short definition when you hover, tap or focus them.

## Category map

Seven categories, ordered so that each depends only on the ones before it. Every item lives in one place.

1. [Framework](#framework)
   - [Structure](#structure)[Layer stack](#layer-stack), [Growth stages](#growth-stages), [Buy-and-build split](#buy-build)
   - [Sequence](#sequence)[Bring-up order](#bring-up), [Sequencing disputes](#disputes)
   - [Roadmap](#roadmap)[Master grid](#master-grid), [Contracts](#contracts), [Learning path](#learning)
2. [Foundation](#foundation)
   - [Runtime](#runtime)[Platform layer](#platform-layer), [Memory](#memory), [Concurrency](#concurrency)
   - [Core utilities](#core-utilities)[Math](#math), [Containers, strings, hashing and logging](#containers), [Random numbers](#rng)
   - [Time and settings](#time-and-settings)[Game loop and time](#loop), [Configuration and CVars](#config)
   - [Files](#files)[VFS and archives](#vfs), [Compression](#compression)
   - [Data](#data)[Asset pipeline and resources](#assets), [Serialization and saves](#serialization)
3. [Subsystems](#subsystems)
   - [Rendering](#rendering)[Rendering core](#rendering-core), [GL-class API limits](#gl-limits), [Lighting and shadows](#lighting), [Atmosphere and post-processing](#atmosphere), [Particles and VFX](#particles), [Terrain, LOD and occlusion](#terrain)
   - [Motion](#motion)[Physics and collision](#physics), [Animation](#animation)
   - [Media](#media)[Audio](#audio), [Video playback](#video)
4. [Gameplay](#gameplay)
   - [Logic](#logic)[Entity and object model](#entities), [Events and messaging](#events), [Scripting](#scripting)
   - [World](#world)[Scene and world management](#scene), [Cameras](#cameras), [AI, perception and navigation](#ai), [Procedural generation](#procgen)
5. [Interface](#interface)
   - [Interaction](#interaction)[Input](#input), [UI](#ui)
   - [Language](#language)[Text and fonts](#text), [Localization](#localization)
   - [Access](#access)[Accessibility](#accessibility)
6. [Tools](#tools)
   - [Authoring](#authoring)[Editor](#editor), [Hot reload](#hot-reload)
   - [Diagnostics](#diagnostics)[Debug tools and profiling](#debug), [Crash reporting](#crash), [Telemetry](#telemetry)
   - [Delivery](#delivery)[Automated testing](#testing), [Build, CI and release](#build)
7. [Online](#online)
   - [Multiplayer](#multiplayer)[Networking and hosting](#networking), [Anti-cheat and security](#anticheat)
   - [Distribution](#distribution)[Platform services](#platform-services), [Patching, updates and DLC](#patching), [Modding and UGC](#modding)
   - [Compliance](#compliance)[Console porting and certification](#console), [Legal, licensing and privacy](#legal)

- [Connections](#connections)
- [Open questions](#open-questions)
- [Sources](#sources)

## CategoryFramework

The whole roadmap hangs on three ideas: a layer stack with downward-only dependencies, five growth stages closed by exit gates, and a list of contracts to lock early.

### Structure

#### Layer stack

Every engine converges on the same layer stack.

What it is

The layer stack is the order in which engine code is stacked, from hardware and the operating system at the bottom to game-specific code at the top. Each layer may call only the layers beneath it.

Key findings Established sourced, with one inferred rule

Jason Gregory's *Game Engine Architecture*, now a 4th edition in two volumes ([gameenginebook.com TOC](https://www.gameenginebook.com/toc.html)), stacks the runtime as follows ([UMD CMSC425, paraphrasing Gregory](https://www.cs.umd.edu/class/spring2018/cmsc425/Lects/lect02-architecture.pdf)):

1. Hardware and OS.
2. Third-party SDKs.
3. A platform independence layer.
4. Core systems: assertions, unit testing, memory allocation, math, parsers and file I/O.
5. A resource manager.
6. The renderer, with peer subsystems for physics, animation, input, audio and networking.
7. A gameplay foundation layer: object model, events, scripting and AI.
8. Game-specific code.

Production engines enforce the split mechanically:

- Godot's rendering, physics and audio servers sit beneath the scene system and hand out opaque RIDs ([Godot docs](https://docs.godotengine.org/en/stable/_sources/tutorials/performance/using_servers.rst.txt)).
- Unreal modules expose Public headers and hide Private ones. "Private dependencies are preferred wherever possible, as they can reduce your project's compile times" ([Unreal Engine Modules](https://dev.epicgames.com/documentation/en-us/unreal-engine/unreal-engine-modules)).
- Bevy implements every feature as a plugin, so dropping `RenderPlugin` yields a headless build ([Bevy plugins](https://bevy.org/learn/quick-start/getting-started/plugins/)).

The one rule worth enforcing is that **dependencies point downward only**. The cheapest enforcement is one build target per layer with private links, so an upward include becomes a build error instead of a review comment.

Startup and shutdown also need an explicit order. C++ constructs static objects in an arbitrary order, so course material built on Gregory recommends explicit `startUp()`/`shutDown()` calls in dependency order, reversed on exit ([Taylor Univ. COS370](https://gfx.cse.taylor.edu/courses/cos370-game//slides/02_Systems.md.html)). Mark Claypool's Dragonfly teaching engine makes the **log manager checkpoint #1**, because every other manager logs during startup ([Dragonfly checkpoint #1](https://web.cs.wpi.edu/~claypool/misc/dragonfly/book/pdfs/dev-checkpoint-1.pdf)).

Why it matters

It gives every later subsystem a fixed place and one enforceable rule: dependencies point downward only.

[Back to the category map](#category-map)

#### Growth stages

Five growth stages, each closed by an exit gate.

What it is

Five growth stages describe what an engine must do as its project grows. Each stage ends at an exit gate, which is a testable “done when” condition.

Key findings Inference the report's synthesis

| Stage | Purpose | Exit gate ("done when") | Trigger to advance | Do not leave without |
| --- | --- | --- | --- | --- |
| S1 Prototype / first playable | Prove the core loop. Build only what the game needs | One vertical slice plays start to finish at the target frame rate. The sim runs on a fixed tick. Every load goes through engine APIs | A decision to ship (store page, publisher deal, public demo) | A conventions doc (axes, units, depth, handedness), the fixed-tick seam, generational handles, action-based input, string keys, UTF-8 everywhere |
| S2 First shipped game, one platform | Ship and patch one game on one store | Released. Crash reports arrive symbolized. A one-asset patch is small. Old saves load in new builds. CI produces the release build | A second platform, a console deal or a second store | Crash capture and symbols, deterministic cooks and a stable pack layout, versioned saves and settings, consent gating, reproducible builds |
| S3 Multi-platform / console-ready | The same game on several platforms | Every platform builds and tests nightly from one commit. Lifecycle, multi-user identity and per-platform cooks pass a pre-certification self-test | A second title, or more than a few programmers | Platform-holder agreements. Everything console-specific depends on NDA material |
| S4 Growing team, multiple titles | The engine becomes a product others use | A new title integrates subsystems by data, not engine code. Non-programmers ship content without engineers. Data merges cleanly | A persistent online world or a live-service cadence | Versioned engine modules, binary-asset version control with locks, shared caches, written contracts |
| S5 Large-scale / online | Server fleets, persistent state, live operations | Capacity numbers come from repeatable load tests. All valuable state is server-authoritative. Ops can tune live servers without a deploy | — | Server authority, privacy-request pipelines, moderation |

Why it matters

The stages are the columns of the whole roadmap, so every subsystem's plan is read against them.

[Back to the category map](#category-map)

#### Buy-and-build split

Buy the leaf algorithms, write the seams, and lock what leaks into data.

What it is

The buy-and-build split is the report's rule for dividing work. Buy leaf algorithms that sit behind an interface, and write the seams (the interfaces and formats between systems) yourself.

Key findings Inference the report's synthesis

**Buy the leaves.** The sources agree on how to split the work. A leaf is an algorithm that sits behind an engine interface:

- Jolt for physics
- Recast/Detour for navigation
- ozz and ACL for animation
- FreeType and HarfBuzz for text
- Crashpad or Sentry for crash capture
- meshoptimizer and KTX for cooking
- Dear ImGui for tools
- a store SDK or EOS for platform services

**Write the seams yourself.** Handles and stable IDs, reflection, the save and pack formats, the input action layer, the render interface, replication, and the tool glue.

**Lock what leaks.** The expensive decisions are the ones that leak into data or into contracts with other people. Almost all of them cost little to lock in stages 1 and 2; the full list is under [Contracts](#contracts).

**Defer the headline features.** Most belong to stages 4 and 5 and should wait for a measured trigger: hardware ray tracing, mesh shaders, motion matching, sandboxed script VMs, world-cell streaming, collaborative editing and client anti-cheat.

**Read “from scratch” narrowly.** In 2026 it means owning interfaces and formats while renting algorithms. The engine's own code concentrates where no library reaches.

Why it matters

It decides where a small team's engineering time goes, and every subsystem's build-vs-buy call applies it.

[Back to the category map](#category-map)

### Sequence

#### Bring-up order

Inside stages 1–2, systems come up in a fixed dependency order.

What it is

Bring-up order is the sequence in which systems are first built during stages 1 and 2. The report labels the steps P0 (tooling) to P10 (concurrency).

Key findings Inference phase table is synthesis; the two source timelines are Established

Two kinds of source describe the build order:

- **Solo timeline.** The best-documented one is Handmade Hero. It spends roughly **25 episodes on the platform layer** (window, input, sound, timing, live code reload) before "Day 026: Introduction to Game Architecture," out of **667 days** in total ([Handmade Hero Episode Guide](https://guide.handmadehero.org/)).
- **Textbook order.** Gregory's chapters run tools, C++ and hardware, parallelism, 3D math, engine support systems, the resource manager, the game loop, input and debugging. Rendering, animation, physics, audio and gameplay come only after that, in Volume II ([gameenginebook.com TOC](https://www.gameenginebook.com/toc.html)).

Josh Petrie argues from the other end: "The notion that you must have an engine to build a non-trivial game is a fallacy." Build a game, extract the reusable code, and test it against the finished game ([Petrie, "Write Games, Not Engines"](https://geometrian.com/projects/blog/write_games_not_engines.html)). The approaches agree on bottom-up dependency order and disagree on whether the engine leads or follows the game. The phases below follow dependency order, gated by what the game needs.

| Phase | Build | Needs first | Done when |
| --- | --- | --- | --- |
| P0 Tooling baseline | Version control, one build target per layer, CI on every target desktop OS, warnings as errors, sanitizer preset, profiler zones | — | One-command clean build. CI is green. A profiler capture shows frame zones |
| P1 Platform layer | Window, event pump, high-resolution timer, paths, file I/O, raw input devices behind an engine API | P0 | The loop runs with no OS or platform-library types in gameplay headers |
| P2 Core systems | Log/assert, config and CVars, math with a conventions doc, containers, string IDs, allocators, handles, RNG, byte streams | P1 | Every subsystem logs, asserts and allocates through engine interfaces. Conventions tests pass. Anything an existing thread touches is thread-safe or thread-confined |
| P3 Loop and simulation seam | Fixed tick with accumulator and interpolation. Simulation / Presentation / Input split. Input command stream. Headless flag | P2 | The game runs identically with rendering off. Recorded commands replay to the same result. No gameplay code reads wall-clock time |
| P4 Data layer | VFS read path, resource manager with handles and GUIDs, reflection with field flags, text scene format, crash-safe writes, offline cook v1 | P2–P3 | No code reads files directly. Any component dumps through reflection alone. The shipping runtime links no source-format parsers |
| P5 Peer subsystems | Renderer behind an explicit-style interface, audio mixer, physics behind an engine API, animation runtime, input actions, debug overlay and debug draw | P4 | The vertical slice holds the frame budget with per-pass GPU timings. A stress test produces zero audio underruns |
| P6 Gameplay foundations | Entity API seam, events with fixed flush points, hierarchy and transforms, camera stack, AI and perception, navmesh, prefabs, spatial index, persistent IDs | P5 | Gameplay compiles against engine APIs only. Saves round-trip through stable IDs |
| P7 Player I/O | UI framework, shaping text service, string tables, accessibility basics, versioned settings file | P5–P6 | Every screen works gamepad-only and mouse-only. Pseudo-localization is clean. A basic accessibility checklist passes |
| P8 Ship pipeline | Crash reporter and symbol upload, deterministic packs, platform-services backend, notices screen, scripted release upload | P4–P7 | A tagged commit produces a store beta build with symbols in the crash backend |
| P9 Online (if the design needs it) | Transport, replication, prediction, lag compensation, headless server | P3, P4, P6 (stable IDs) | N clients stay consistent across a network-condition matrix |
| P10 Concurrency (trigger-gated) | Job API, parallel ECS scheduling, async I/O, per-thread arenas | Profiler shows CPU-bound frames | Parallel results match serial results with no frame regression |

The phases are a dependency order, not a calendar. P3's seam needs nothing from rendering, so it can be cut early. P10 waits for a profiler trigger, but thread-safety for threads that already exist (loader, audio callback, log backend) starts in P2.

Why it matters

It answers “what do I build next?” for the period before the first ship.

[Back to the category map](#category-map)

#### Sequencing disputes

The sources split on five sequencing questions.

What it is

Five questions on which the report's sources give different build orders, each with the report's own call.

Key findings Contested both sides sourced; the generic call is Inference

| Question | One side | Other side | Generic call (inference) |
| --- | --- | --- | --- |
| Logging or window first? | Dragonfly builds the log manager first ([Dragonfly](https://web.cs.wpi.edu/~claypool/misc/dragonfly/book/pdfs/dev-checkpoint-1.pdf)) | Handmade Hero opens with the window and formalizes debug file I/O on Day 15 ([HMH](https://guide.handmadehero.org/)) | Both land in P1–P2. The order between them barely matters  Decided 14 Nov 2026 Logging first. The log manager comes up before the window, so errors are caught in every development build, and each log call records a timestamp. Inference Take the timestamp from the monotonic clock, and add the tick number once the fixed tick exists in P3, so log lines can be ordered against the simulation. |
| Audio early or late? | Handmade Hero plays sound by Days 7–9 ([HMH](https://guide.handmadehero.org/)) | Gregory puts audio in Volume II, after rendering, animation and physics ([TOC](https://www.gameenginebook.com/toc.html)) | Device and mixer in P5 alongside rendering. Earlier if the design depends on sound  Decided 14 Nov 2026 Audio late. Rendering comes first, and the audio device and mixer follow it. Inference The audio half of the P5 exit gate (a stress test with zero underruns) moves with it. Gameplay noise events do not depend on the mixer, so AI that hears can still be built on schedule (see [Audio](#audio)). |
| Hot reload early? | Handmade Hero builds live code reload on Days 21–23, before any game architecture ([HMH](https://guide.handmadehero.org/)) | Gregory's TOC does not single it out | Config, shader and asset reload in S1. Code reload only when the edit-restart loop is a measured top time sink  Decided 14 Nov 2026 Yes, hot reload early. It is built alongside the logging system, so reloads can be tested and their errors checked from the start. Inference Config, shader and asset reload are already stage 1 work, and each reload should log its success or failure. Early code reload has one precondition: gameplay sits behind a module boundary (see [Hot reload](#hot-reload)). |
| Jobs foundational or retrofit? | Our Machinery: "Everything after the initial system boot-up code should be executed and scheduled through the job system" ([Our Machinery](https://ruby0x1.github.io/machinery_blog_archive/post/fiber-based-job-system)) | Naughty Dog retrofitted a fiber job system to reach 60 fps on PS4 ([GDC Vault](https://gdcvault.com/play/1022187/Parallelizing-the-Naughty-Dog-Engin)) | Thread-safe core primitives from P2. The job system arrives when the profiler trips  Decided 14 Nov 2026 Foundational jobs. The engine job API is built with the core in P2, and every subsystem schedules its work through it. This overrides the report's call. Inference P10 keeps the rest of its list: parallel ECS scheduling, async I/O and per-thread arenas. The physics library plugs into the engine job API at P5, which removes the two-scheduler stopgap. Keep a one-worker serial mode, so that “parallel output equals serial output” is a standing test (see [Concurrency](#concurrency)). |
| Engine first or game first? | Gregory and Dragonfly are engine-first | Petrie: extract the engine from shipped games ([Petrie](https://geometrian.com/projects/blog/write_games_not_engines.html)) | Game first. A phase is done when the game runs on it  Decided 14 Nov 2026 Piece by piece. A minimal engine comes first, so the game has a way to run. After that, the engine is built from what the game needs. Inference In roadmap terms the baseline is the smallest slice that runs a game loop: P0 to P3, plus a first draw and input from P5. Past that point the game's next need picks the next phase, the dependency order still limits which phases are unblocked, and each phase is done when the game runs on it. |

Why it matters

They mark where the bring-up order is a judgment, so a reader knows which parts to revisit for their own project.

[Back to the category map](#category-map)

### Roadmap

#### Master grid

One table, every subsystem.

What it is

One table that places all 47 subsystems on the five stages. Each cell names the implementation that fits that stage.

Key findings Inference the report's synthesis

Each cell is the implementation that fits that stage. The subsystem sections below give the gates and triggers.

| Subsystem | S1 Prototype | S2 First ship, one platform | S3 Multi-platform / console | S4 Team, multiple titles | S5 Large-scale / online |
| --- | --- | --- | --- | --- | --- |
| Foundation |  |  |  |  |  |
| [Platform layer](#platform-layer) | Library wrapper: window, events, timer | Two desktop OSes; focus-loss and disconnect pause | Lifecycle state machine; multi-user identity; console ports | Versioned module with conformance tests | Headless server platform |
| [Memory](#memory) | Named, capped arenas and pools; handles; profiler and ASan hooks | Third-party allocators routed; budgets asserted | Per-platform budgets; per-thread arenas | Budget regressions fail CI | Per-shard arenas; multi-day soaks |
| [Concurrency](#concurrency) | One sim thread plus loader, audio and log threads; annotated locks; TSan | Engine job API over a bought scheduler; ECS read/write scheduling | Per-platform thread tables; P/E-core pools | Whole-frame job graph with access validation | Isolated arenas or a process per shard |
| [Math](#math) | Library behind aliases plus conventions tests | SIMD hot paths; no fast-math in the sim | Cross-compiler golden tests | Stable math layout in files and packets | Large-world coordinates; fixed point if bit-exact |
| [Containers, strings, hashing, logging](#containers) | pmr containers; compile-time string IDs; UTF-8 | Flat maps; ID collision check; per-thread log queues | Own containers if the platform STL hurts | Frozen, versioned ID spec | 64-bit IDs across services; central structured logs |
| [Loop and time](#loop) | Fixed tick plus interpolation; one monotonic clock | Frame limiter; real time vs game time | Per-platform present pacing; injectable clock | Tick-based timeline; wall clock linted out of the sim | Client-server tick sync with slew |
| [Config and CVars](#config) | One file plus command line; CVar registry | Layered config; flags; separate per-device file | Per-platform layers; validation | Schema, hot reload, generated docs | Remote config and audited feature flags |
| [Random numbers](#rng) | Small PRNG, SplitMix seeding, own range mapping | Named streams; full state saved | Cross-platform golden outputs | Versioned algorithms; seeds in crash reports | Server-only authoritative RNG; CSPRNG for security |
| [VFS and archives](#vfs) | Engine Path type; one File API | Ordered mounts; patch-friendly pack v1 | Platform async I/O; chunk IDs | Overlay priority rules; sandboxed mod mounts | Content-addressed CDN chunks |
| [Compression](#compression) | None, or zstd | zstd per asset; LZ4 for streaming | GPU or hardware decompression | Trained dictionaries; texture RDO | Packet and storage compression |
| Foundation · Data |  |  |  |  |  |
| [Asset pipeline and resources](#assets) | Direct load, then an offline cook CLI; handle cache | Incremental cook; GUID sidecars; packs; async loads | Per-platform cook | Shared derived-data cache | Distributed cook; on-demand chunks |
| [Serialization and saves](#serialization) | Byte streams; reflection with flags; atomic writes | Versioned chunked save v1; golden saves in CI; cloud sync | Storage abstraction; console save rules | Schema registry; reflection codegen | Server persistence owns progression |
| Subsystems |  |  |  |  |  |
| [Rendering core](#rendering-core) | Explicit-style render interface on one backend; offline shaders; PBR | Cooked meshes; post chain; culling and batching; shader reload | Render graph; second explicit backend; per-platform shaders | Shared renderer; artist materials; golden images | GPU-driven, bindless, mesh shaders, ray tracing |
| [Lighting and shadows](#lighting) | Photometric units; pre-exposure; first shadowed lights | Shadow atlas with caching; clustered lists; baked/probe GI; AO | Per-platform quality tiers | Headless bake farm | Sun cascades; streamed probes; runtime RT GI |
| [Atmosphere and post](#atmosphere) | HDR target; manual exposure; one tonemapper | Fog, bloom, grain, froxel volumetrics, TAA or SMAA, LUTs | Vendor upscalers; dynamic resolution | Shared post/LUT library; OCIO configs | Physical sky; weather-driven fog |
| [Particles and VFX](#particles) | CPU billboards | Soft and lit particles; decals; GPU particles; module stack | Per-platform budgets | Node-graph authoring; effect library | Effect LOD; cosmetic effects off the sim path |
| [Terrain, LOD, occlusion](#terrain) | Static levels; frustum culling | Mesh LOD; portals or Hi-Z occlusion | GPU-driven culling; per-platform LOD bias | HLOD and impostor cooking | Terrain, virtual texturing, cluster LOD |
| [Physics and collision](#physics) | Library with sample job pool; raycasts; kinematic character | Engine physics API; layers and materials as data; CCD; ragdolls | Console builds; optional cross-platform determinism | Pinned, vendored library; CI determinism | Physics per zone; doubles; server authority |
| [Animation](#animation) | glTF import; GPU skinning; crossfade | Blend spaces, layers, data-driven state machine, root motion, events, IK | Compression; parallel sampling; LOD | Graph editor; retargeting | Crowd LOD; server evaluates root motion and hitboxes only |
| [Audio](#audio) | Device and real-time mixer behind a facade | Streaming, voices, buses, 3D, occlusion, gameplay noise events | Console backends; HRTF | Designer-authored sound events | No audio on servers; voice at scale |
| [Video playback](#video) | None, or a single-header decoder to a texture | Cutscenes with A/V sync and captions; license checklist | Commercial or platform decoders | Encode presets in the cook | CDN-streamed menu video |
| Gameplay |  |  |  |  |  |
| [Entity/object model](#entities) | Entity API seam over objects or an ECS | ECS library behind the API; persistent IDs | Parallel scheduling; command buffers | Documented hybrid or in-house store | Per-shard worlds; push change tracking |
| [Events and messaging](#events) | Scoped observers; one gameplay→presentation queue | Typed queues flushed at fixed tick points; commands | Deterministic per-thread merge | Event tracing; written ordering contract | Interest-managed delivery |
| [Scripting](#scripting) | Data-driven C++; config reload; optional code reload | Optional embedded language for level logic | Interpreter or AOT on consoles | Typed scripts; visual tools | Sandboxed server and UGC scripts; signed bytecode |
| [Scene and streaming](#scene) | Flat scene; hierarchy with dirty-flag transforms | Prefabs; spatial index; async level loads | Per-platform cooked scenes; sub-levels | One file per entity | Cell streaming with interest management |
| [Cameras](#cameras) | First-person view plus debug fly-cam | Camera stack, event-driven shake, priority brain and blends | Per-platform input curves; comfort options | Sequencer; reusable rig assets | Spectator and replay cameras from state |
| [AI, perception, navigation](#ai) | FSM with reason strings; perception; waypoints | HFSM or BT; navmesh; a director if the genre needs it | AI LOD; sliced pathfinding | Data-driven trees; editors; AI replay | Server-only AI; tile streaming; crowds |
| [Procedural generation](#procgen) | Own PRNG; seeded assembly with a guaranteed path | Validators; stream separation; seed tools | Cross-platform hash as a release gate | Template editor; WFC or grammar fill | Server-authoritative generation; hash check on join |
| Interface |  |  |  |  |  |
| [Input](#input) | Device state plus a thin action table; scancodes; hotplug | Data-driven actions and contexts; rebinding; glyphs | Per-platform backends; user/controller pairing | Designer-editable action assets; replay tests | Server-validated command streams |
| [UI](#ui) | Debug UI for tools; minimal game menu | One shipping UI framework; focus navigation; scaling | Safe areas; platform screens | Designer markup and data binding; narration tree | Virtualized lists; server-driven content |
| [Text and fonts](#text) | Bitmap atlas; UTF-8 | FreeType + HarfBuzz; MSDF; CJK fallback | Bidi and mirrored UI | One text service; font subsetting | Any user script; color emoji |
| [Localization](#localization) | `loc(key)` for every string | Context, CLDR plurals, pseudo-loc | Localized assets and VO; ICU formatting | Translation pipeline; string freeze | Hot-updatable strings; key+args over the wire |
| [Accessibility](#accessibility) | Hooks: actions, audio buses, subtitle data model | Basic guideline tier; subtitle spec; flash limits | Full console-guideline pass; screen narration | Shared accessibility library; profiles | Speech-to-text and text-to-speech chat |
| Tools |  |  |  |  |  |
| [Editor](#editor) | Debug overlay with reflected inspector | Gizmos, undo, play-in-editor for repeated workflows | PC-only editor; platform preview through cooking | One file per entity; separate editor process | Collaborative data model |
| [Hot reload](#hot-reload) | Config, shader and asset reload | Optional code reload | Remote file serving to devkits | Network reload for artists | Live server config and data |
| [Debug tools](#debug) | Log categories, assert trio, CVars, console, overlay, debug draw | GPU capture hook; shipping stripping | Platform log sinks and overlays | Generated CVar docs; perf captures in CI | Authenticated remote console |
| [Crash reporting](#crash) | Local dumps | Out-of-process reporter; symbols per build; triage | Console crash SDKs | Triage rotation; symbol-store governance | Server core dumps |
| [Telemetry](#telemetry) | Local playtest logs | Opt-in, minimal schema; perf histograms | Platform privacy rules | Schema registry; retention policy | Streaming ingestion; experiments |
| [Automated testing](#testing) | Unit tests; headless smoke test | Golden images; replays; nightly perf budgets | Per-platform buckets; devkit runners | UI and tool tests; flaky-test quarantine | Bots, soak and load tests |
| [Build, CI, release](#build) | Presets and a pinned dependency manifest | Two-OS CI with caches; versioned release upload | Toolchains for every platform; nightly builds | Binary VCS with locks; trunk-based review | Distributed builds; parallel patch lines |
| Online |  |  |  |  |  |
| [Networking and hosting](#networking) | Sim seam; command stream; LAN test | Listen server; snapshots and deltas; prediction | Dedicated headless servers; relays | Relevancy and priority; shared netcode | Spatial relevancy; multi-process sims; persistent backend |
| [Platform services](#platform-services) | Interfaces plus a Null backend | One store backend | Second store or cross-play backend; consoles | Shared module with conformance tests | First-party accounts; server-written stats |
| [Modding and UGC](#modding) | Data-driven content; dev override folder | Mods folder with manifest; no code mods | Workshop-style integration; UGC privileges | Mod SDK; sandboxed scripting | Server validation and moderation |
| [Patching and DLC](#patching) | Deterministic cook | Patch-friendly packs; beta branches | Entitlements per store; DLC mounts | Patch-size regression checks | Server-side entitlements; staged rollouts |
| [Anti-cheat and security](#anticheat) | Debug commands gated | Shipping hardening; auth tickets | Trusted leaderboards; evaluate anti-cheat | Shared ban and report tools | Server-owned value; client anti-cheat as one layer |
| [Console porting and cert](#console) | No single-user globals | PC equivalents of cert behaviors | Platform access; port; pre-cert self-test | Devkits in CI; per-platform cert tracker | Service rate limits; cross-network privileges |
| [Legal and privacy](#legal) | License manifest | Notices screen; age rating; consent | Ratings per store; privacy policy | License scanning in CI | GDPR/COPPA request pipelines |

Why it matters

It is the roadmap itself: one row per subsystem and one column per stage.

[Back to the category map](#category-map)

#### Contracts

Eighteen contracts to lock before they leak into data.

What it is

A contract here is a decision that content, saves, network packets, patches or other people come to depend on. The report ranks eighteen of them by how far they leak.

Key findings Established reasons are sourced; the “lock by” and “rule” columns are Inference

The pattern across every subsystem is consistent. A decision is expensive when content, saves, packets, patches or other people come to depend on it. A library hidden behind an engine interface is cheap to swap. The table ranks the contracts roughly by how much they leak.

| # | Contract | Lock by | Why reversal is expensive (sourced) | Rule |
| --- | --- | --- | --- | --- |
| 1 | Coordinate system, handedness, up axis, units, depth range, matrix order | S1 | GL's default clip volume is lower-left origin with −1..1 depth until `glClipControl` changes it ([Khronos](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glClipControl.xhtml)). Jolt works best in SI units with dynamic objects of 0.1–10 m ([Jolt Architecture](https://github.com/jrouwe/JoltPhysics/blob/master/Docs/Architecture.md)) | Write the conventions doc and its unit tests before the first asset |
| 2 | Fixed-tick simulation seam | S1 (P3) | Prediction "requires a deterministic world" ([Gambetta](https://www.gabrielgambetta.com/client-side-prediction-server-reconciliation.html)). A variable timestep makes behavior frame-rate dependent ([Fiedler](https://gafferongames.com/post/fix_your_timestep/)) | No gameplay code reads wall-clock time or render delta time |
| 3 | Runtime handles vs stable IDs | S1–S2 | EnTT's entity format "can change in future" ([EnTT wiki](https://github.com/skypjack/entt/wiki/Entity-Component-System)). Bevy's `AssetIndex` is "not suitable for identifying assets across runs" ([Bevy](https://docs.rs/bevy/latest/bevy/asset/index.html)) | Files and packets carry only persistent IDs, net IDs or GUIDs |
| 4 | One reflection layer with per-field flags | S1–S2 | "Data serialized without versioning cannot be loaded by a versioned serialization function (and vice versa)" ([cereal](https://uscilab.github.io/cereal/serialization_functions.html)) | Stable name-hash type IDs and field IDs. Flags drive saves, replication and the inspector |
| 5 | String, asset and type ID scheme | S2 | FNV-1a hashed strings can collide, and `type_name` differs between MSVC and GCC/Clang ([EnTT core](https://github.com/skypjack/entt/wiki/Core-Functionalities)) | Fix the algorithm, width, seed and normalization. Version the scheme like a file format |
| 6 | VFS path rules and pack/TOC layout | First public build | SteamPipe diffs in \~1 MB chunks, and with scattered offsets "a change to an 8-byte number will result in a new 1MB chunk" ([Steamworks](https://partner.steamgames.com/doc/sdk/uploading)) | Stable asset order, one TOC at the start or end, per-asset compression, new packs for updates |
| 7 | Versioned, chunked save format | Save v1 | Xbox requires that "A content-updated version of a game must be able to successfully load a save created using the non-content-updated version" (XR-003-16) ([Xbox Requirements](https://learn.microsoft.com/id-id/gaming/gdk/_content/gc/policies/console/certification-requirements)) | Header, chunks and migrators from the very first save |
| 8 | Storage identifiers | First public build | SDL says the app string should "never change ... once you've decided on it" ([SDL_GetPrefPath](https://wiki.libsdl.org/SDL3/SDL_GetPrefPath)) | Freeze org and app identifiers |
| 9 | RNG algorithm, seeding and streams | Before the first save or shared seed | Standard distribution output "is implementation-specified and thus can (and does) differ across implementations" ([P2059R0](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p2059r0.pdf)) | Own PRNG and range mapping. Streams per system. Save the full state |
| 10 | Determinism promise | Before networking or replays | Jolt's cross-platform mode costs \~8% and constrains math and sorting ([Jolt Architecture](https://github.com/jrouwe/JoltPhysics/blob/master/Docs/Architecture.md)). MotoGP couldn't share replays between Xbox and PC ([Fiedler](https://gafferongames.com/post/floating_point_determinism/)) | Choose same-binary, cross-platform or none, explicitly and in writing |
| 11 | Explicit-style render interface and one offline shader language | Before the second backend | SDL_GPU has no OpenGL backend ([SDL3 GPU](https://wiki.libsdl.org/SDL3/CategoryGPU)). On PlayStation, "shaders *must* be precompiled ahead of time" ([FNA](https://fna-xna.github.io/docs/appendix/Appendix-B:-FNA-on-Consoles)) | No graphics-API enum outside the backend |
| 12 | Light units, exposure, working color space, tonemapper | Before art production | Physical units "enable full reusability of lighting rigs". Storing lighting in RGB16F requires pre-exposure ([Filament](https://google.github.io/filament/Filament.html)) | Photometric units, EV100, pre-exposure, one view transform |
| 13 | Action-based input and the bindings format | S1 | Steam Input's native API is "built around the in-game actions & sets that you specified" ([Steam Input](https://partner.steamgames.com/doc/features/steam_controller/getting_started_for_devs)) | Gameplay reads actions, never devices. Bind by scancode plus device class |
| 14 | String keys, whole-message formatting, run-based text API | S1–S2 | With fragments, "translators would not be able to rearrange the pieces" ([ICU](https://unicode-org.github.io/icu/userguide/format_parse/messages/)) | Only `loc(key, args)`. The renderer accepts shaped glyph runs, not characters |
| 15 | Event ordering and flush semantics | S2 | EnTT's immediate dispatch order "is not guaranteed", and connecting listeners in handlers "can result in undefined behavior" ([EnTT signals](https://github.com/skypjack/entt/blob/HEAD/docs/md/signal.md)) | Queued events flush at documented tick points. Events raised in handlers go to the next flush |
| 16 | Player identity model and authority placement | S2 | Xbox mandates user/controller pairing checks and account-linking rules (XR-112, XR-013) ([Xbox Requirements](https://learn.microsoft.com/id-id/gaming/gdk/_content/gc/policies/console/certification-requirements)). Server-set Steam stats "cannot be set by clients" ([Steam Stats](https://partner.steamgames.com/doc/features/achievements)) | An opaque, platform-tagged `PlayerId` plus a local user index. Competitive stats are server-written from day one |
| 17 | Async platform calls; capabilities may be absent | S1 | Unreal's online interfaces may be null or "may simply return false", and async operations always call their delegates ([UE Online Subsystem](https://dev.epicgames.com/documentation/en-us/unreal-engine/online-subsystem-in-unreal-engine)) | All platform calls are async, even on the Null backend |
| 18 | License linking and code-mod policy | Before first ship | LGPL allows "a suitable shared library mechanism" instead of shipping relinkable objects ([LGPL-3.0](https://spdx.org/licenses/preview/LGPL-3.0.html)). A basic Lua sandbox "doesn't handle resource exhaustion issues" ([lua-users wiki](http://lua-users.org/wiki/SandBoxes)) | Link LGPL code dynamically. Allow no native-code mods |

Why it matters

These decisions are cheap in stages 1 and 2 and expensive to reverse later, so they set the early priorities.

[Back to the category map](#category-map)

#### Learning path

What to learn at each stage.

What it is

A reading list per stage: the areas to understand and the anchor sources for each.

Key findings Inference the report's synthesis

| Stage | Areas to understand | Anchor sources |
| --- | --- | --- |
| S1 | Build systems, debugger, profiler, sanitizers; OS event loops and timers; C++ lifetime and memory layout; vectors, matrices and quaternions; fixed timesteps; the graphics pipeline and shaders; real-time audio rules; hashing and Unicode basics | Gregory ch. 2–5 ([TOC](https://www.gameenginebook.com/toc.html)); Handmade Hero Days 1–25; Fiedler; Nystrom; LearnOpenGL; Bencina |
| S2 | Data-oriented design and ECS storage; schema evolution and crash-safe I/O; PBR and photometry; shadows and post; physics tuning (constraints, CCD, determinism); skinning, blending and IK; navmeshes and behavior trees; text shaping and CLDR plurals; accessibility guidelines; minidumps and symbols; store tooling | Acton; Fabian; FlatBuffers and Protobuf rules; Filament; Catto's GDC talks; Ericson; the glTF spec; Recast docs; HarfBuzz; GAG; Crashpad; Steamworks docs |
| S3 | Explicit GPU APIs (synchronization, memory, descriptors); render graphs; platform async I/O; cross-compiler floating point; platform lifecycle and certification; console SDKs (NDA) | vkguide; Khronos Vulkan-Guide; Frostbite FrameGraph; io_uring and DirectStorage; Dawson; Xbox Requirements |
| S4 | Tool data models (undo, collaboration); build farms and derived-data caches; designer authoring (graphs, markup); test infrastructure; trunk-based team process | The Truth (Our Machinery); Unreal DDC; trunkbaseddevelopment.com; Niagara; Unreal screenshot comparison |
| S5 | Replication at scale (relevancy, priority, push models); distributed simulation; persistence; server operations (service managers, containers, core dumps); privacy law; anti-cheat threat modelling | Fiedler's state-sync series; Unreal Replication Graph; Riot; EVE; systemd; EDPB; FTC |

Why it matters

It turns the roadmap into a study order for whoever builds the engine.

[Back to the category map](#category-map)

## CategoryFoundation

Almost everything in this layer can be bought; the real engineering is the conventions wrapped around the libraries, and those conventions are what the stage gates test.

Overview

Almost everything in this layer can be bought under a permissive license. The exceptions are Oodle, which is commercial outside Unreal, and Google's marl scheduler, which was archived in April 2026 ([marl](https://github.com/google/marl)). The real engineering is the conventions wrapped around the libraries: thread ownership, coordinate and depth conventions, hash and ID schemes, path and mount rules, pack layout and seeding. Those conventions are what the stage gates test.

### Runtime

#### Platform layer

Own the loop shape first, the lifecycle later.

What it is

The platform layer wraps the operating system: window, events, timer, file paths, input devices and, later, the console lifecycle. Gameplay code talks to it instead of to the OS.

Key findings Established sourced facts, as observed in early November 2026

- **Loop ownership.** SDL3's optional main callbacks (`SDL_AppInit`, `SDL_AppIterate`, `SDL_AppEvent`, `SDL_AppQuit`) exist for platforms that won't let the app own the loop, and SDL emulates them elsewhere ([SDL3 main functions](https://wiki.libsdl.org/SDL3/README-main-functions)).
- **Main thread.** `SDL_CreateWindow` "should only be called on the main thread" ([SDL wiki](https://wiki.libsdl.org/SDL3/SDL_CreateWindow)). `SDL_RunOnMainThread` (since 3.2.0) marshals work back to it ([SDL wiki](https://wiki.libsdl.org/SDL3/SDL_RunOnMainThread)).
- **Xbox.** SDL's GDK backend calls suspend/resume "Required for Xbox certification." The game handles background/foreground events, suspends GPU work and calls `SDL_GDKSuspendComplete`. The callbacks arrive on a background thread. The backend needs Visual Studio 2022 and GDK April 2026 Update 3 or newer, and only licensed developers get the GDKX libraries ([SDL3 README-gdk](https://wiki.libsdl.org/SDL3/README-gdk/raw)).
- **Switch.** The Switch and Switch 2 port is zlib-licensed "but only for people under the appropriate NDAs with Nintendo" ([SDL3 README-switch](https://wiki.libsdl.org/SDL3/README-switch/raw)).
- **Xbox Requirements.** After suspend, "The game doesn't fully restart" (XR-001-02). Consoles suspend a game that "remains out of focus for ten minutes" ([Xbox Requirements v16.7](https://learn.microsoft.com/id-id/gaming/gdk/_content/gc/policies/console/certification-requirements)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Platform library (SDL3 or raw OS APIs) behind an engine API for window, events, timer, paths, file I/O and devices. The loop is shaped as init/iterate/event/quit even inside `main()` | No OS or library types appear in gameplay headers | A second desktop OS, or a ship decision |
| S2 | Two desktop OSes from one tree. Pause on focus loss and on controller disconnect. Per-user writable paths. Main-thread confinement of window and GPU context documented | The same commit runs on both OSes. Focus-loss and disconnect tests pass | A console deal |
| S3 | Lifecycle state machine (active, constrained, suspended, resumed) that quiesces GPU, audio and network. N local users, each with a controller association. Console backends via platform ports | Suspend/resume soak tests pass. A PC debug toggle simulates suspend | A second title |
| S4 | Platform layer as a versioned module with a shared conformance suite | A new backend passes the suite with no engine changes | Server fleets |
| S5 | A server platform target with no window, GPU or audio, plus service-manager integration | The server binary links no presentation code | — |

Guidance Inference the report's synthesis

**Build vs buy.** Buy the portability layer. SDL3 (zlib) covers desktop and, under NDA, Xbox and Switch. Build the engine-facing API and the lifecycle state machine.

**Lock early.** The callback-shaped loop, main-thread confinement as a documented invariant, and the rule that code never assumes a single user.

**Pitfalls.**

- Window or context calls from worker threads.
- Assuming the app owns the loop.
- A global "the user".
- Stopping presentation while paused, which breaks store overlays (see [Platform services](#platform-services)).

**Learn.** OS event models, high-resolution timing, Handmade Hero's platform episodes, and the public Xbox Requirements as a lifecycle checklist.

Why it matters

It is the first engine code written (P1), and its loop shape and single-user assumptions are hard to change once a console port begins.

[Back to the category map](#category-map)

#### Memory

Budgets and handles from the first allocator.

What it is

Memory management decides how the engine allocates: arenas (blocks freed all at once), pools, a general heap, and handles (checked IDs used in place of pointers).

Key findings Established sourced facts, as observed in early November 2026

- **Standard primitives.** C++17's `<memory_resource>` already ships the building blocks ([cppreference](https://en.cppreference.com/w/cpp/header/memory_resource)):
  - an arena: `monotonic_buffer_resource` "releases the allocated memory only when the resource is destroyed"
  - a `null_memory_resource()` upstream that turns arena overflow into a hard failure
  - synchronized and unsynchronized pool resources
- **Bounded heap.** TLSF runs at "an asymptotic constant cost, O(1)," in "less than 200 processor instructions" ([Masmano et al. 2008](https://www.cs.york.ac.uk/rts/publications/Masmano2008.html)).
- **Tracking.** Tracy tracks named pools (`TracyAllocN`), arena resets (`TracyMemoryDiscard`) and a leak list at exit. It "will terminate the profiling session" on a free with no matching allocation, and untracked library allocations are a named cause ([Tracy manual](https://github.com/wolfpld/tracy/blob/master/manual/tracy.tex)).
- **Sanitizers.** ASan's "Typical slowdown" is 2x ([Clang ASan](https://clang.llvm.org/docs/AddressSanitizer.html)). Custom arenas must poison themselves, and the sanitizer wiki warns that plain malloc/free often finds heap bugs better than a custom allocator with manual poisoning ([sanitizers wiki](https://github.com/google/sanitizers/wiki/AddressSanitizerManualPoisoning)).
- **Handles.** Under floooh's rules, systems are "the sole owner of their memory allocations" and generation checks catch stale handles. Plain-data handles work "across processes, useful for networked games and savegames" ([floooh](https://floooh.github.io/2018/06/17/handles-vs-pointers.html)). Bitsquid's ID lookup table was "about 40x faster" than an STL map ([Bitsquid](https://bitsquid.blogspot.com/2011/09/managing-decoupling-part-4-id-lookup.html)).
- **Library allocators.** EASTL's rationale says "Any memory a library allocates must be controllable by the user" ([N2271](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2271.html)), and current libraries comply:
  - Jolt routes allocations through hooks and wants a temp allocator, where 10 MB "is a typical value" ([Jolt HelloWorld](https://github.com/jrouwe/JoltPhysics/blob/master/HelloWorld/HelloWorld.cpp)).
  - ACL states "All allocations use a game provided allocator" ([ACL](https://github.com/nfrechette/acl)).
  - Recast routes allocations through `rcAllocFunc`/`dtAllocFunc` ([Recast](https://github.com/recastnavigation/recastnavigation/blob/HEAD/Docs/_2_BuildingAndIntegrating.md)).
- **Console budgets.** No public source gives them; they are under NDA.

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Four allocators as memory resources: a per-frame arena with a null upstream, fixed pools, a level stack and a general heap. Each has a name, a cap, a high-water mark, profiler hooks, ASan poisoning and a switch to plain malloc. One generational handle template serves entities, GPU resources and voices | Zero global-heap calls in a steady-state frame. Every allocation maps to a named allocator. Stale handles assert in debug | Third-party libraries or worker threads start allocating |
| S2 | Every library routed through its allocator hooks. Per-subsystem budgets assert in debug and log in release. No raw pointers are stored across frames | The profiler sees no unmatched frees. The leak list is empty at exit. Budgets are documented per subsystem | A fixed-memory platform |
| S3 | Per-platform budget tables. Per-thread arenas for job workers. One GPU memory allocator per device (VMA wants exactly one per `VkDevice` ([VMA](https://gpuopen-librariesandsdks.github.io/VulkanMemoryAllocator/html/quick_start.html))) | The weakest target holds its budget through a soak test | Several titles share the engine |
| S4 | Shared allocator library. Per-level high-water marks recorded in CI | A budget regression fails CI | Long-running servers |
| S5 | Per-shard arenas, NUMA-aware placement, fragmentation monitoring over long uptimes | Memory stays flat over a multi-day server soak | — |

Guidance Inference the report's synthesis

**Build vs buy.** The standard library supplies the primitives, TLSF the bounded heap, and Tracy and ASan the visibility. Build the handle tables, the budget policy and the routing glue.

**Lock early.** Handles instead of pointers across frames, and one memory-resource interface that every subsystem allocates through.

**Pitfalls.**

- Untracked library allocations, which abort profiler sessions.
- Arenas that hide bugs from ASan.
- `shared_ptr` as the ownership model.
- Budgets bolted on after content already exists.

**Learn.** Object lifetime and RAII, placement new, alignment and cache lines (Gregory ch. 3), and Acton and Fabian on data-oriented design.

Why it matters

Budgets and handles added with the first allocator avoid a retrofit once content and third-party libraries are in place.

[Back to the category map](#category-map)

#### Concurrency

Own the API, rent the scheduler.

What it is

Concurrency covers which threads exist, what each may touch, and the job system that spreads work across CPU cores.

Key findings Established sourced facts, as observed in early November 2026

- **Studio precedents.**
  - Naughty Dog adopted a fiber-based job system to run *The Last of Us Remastered* at 60 fps on PS4 ([GDC Vault](https://gdcvault.com/play/1022187/Parallelizing-the-Naughty-Dog-Engin)).
  - Bungie turned "almost every part of our engine into a job graph" for Destiny. It added runtime validation that "all data accesses (reads, writes, creations and destructions) are valid" ([GDC Vault](https://gdcvault.com/play/1022164/Multithreading-the-Entire-Destiny)).
  - Our Machinery's counter-based job system was "under 300 lines of code" ([Our Machinery](https://ruby0x1.github.io/machinery_blog_archive/post/fiber-based-job-system)).
  - FiberTaskingLib represents dependencies as atomic counters. It warns that a std mutex held across `Wait()` may resume on another thread ([FiberTaskingLib](https://github.com/RichieSams/FiberTaskingLib)).
- **Buyable schedulers.**
  - enkiTS (zlib): pinned tasks, external threads, "designed for zero allocations during scheduling" ([enkiTS](https://github.com/dougbinks/enkiTS)).
  - Taskflow (MIT): v4 needs C++20 ([Taskflow](https://taskflow.github.io/taskflow/Releases.html)).
  - oneTBB (Apache-2.0): `task_arena` isolation ([oneTBB](https://github.com/uxlfoundation/oneTBB/blob/master/RELEASE_NOTES.md)).
  - Google's marl was **archived on April 27, 2026** ([marl](https://github.com/google/marl)).
- **Queues.** moodycamel's MPMC queue "is not linearizable" and gives no ordering between producers ([concurrentqueue](https://github.com/cameron314/concurrentqueue)).
- **Race tooling.** ThreadSanitizer costs about 5x–15x and does not list Windows ([Clang TSan](https://clang.llvm.org/docs/ThreadSanitizer.html)). Clang's thread-safety analysis is "completely static" with "no run-time overhead" ([Clang TSA](https://clang.llvm.org/docs/ThreadSafetyAnalysis.html)).
- **Hybrid CPUs.** Intel says "avoid hard affinities" and size pools by Performance-core count ([Intel](https://www.intel.com/content/www/us/en/developer/articles/guide/12th-gen-intel-core-processor-gamedev-guide.html)).
- **OpenGL.** Multi-threaded GL calls give no speedup because "Usually there is just 1 GPU" ([Khronos wiki](https://www.khronos.org/opengl/wiki/OpenGL_and_multithreading)).
- **ECS scheduling.** Bevy runs systems in parallel by default and detects conflicts from system parameter types ([Bevy](https://docs.rs/crate/bevy/latest/source/examples/ecs/nondeterministic_system_order.rs)). Flecs stages structural changes in per-thread queues and flushes them at sync points ([Flecs](https://www.flecs.dev/flecs/Systems.html)).
- **Standard.** C++26 standardized `std::execution` as "C++'s async model" ([Sutter](https://herbsutter.com/2026/03/)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | One simulation thread plus dedicated loader, audio and log threads. An engine mutex wrapper with thread-safety annotations. SPSC rings between threads. TSan builds on Linux CI. A written list of what is thread-safe (logging, allocators, VFS reads, CVar reads, profiler markers) and what is main-thread-confined (window, events, GL context) | TSan is clean. The confinement list exists and review enforces it | The profiler shows the main thread over budget with idle cores, or decoding stalls frames |
| S2 | Engine-owned counter/handle API (`Submit → Counter`, `Wait`, `ParallelFor`) over a bought scheduler. Workers sized to P-cores. ECS systems declare reads and writes, and a debug build fails on unordered conflicts. Per-thread command buffers | Parallel output equals serial output. No job allocations in a steady-state frame. Shutdown drains cleanly | Deep wait chains, a render-thread bottleneck, or console ports |
| S3 | Per-platform thread tables (priorities, core reservations). P-core and E-core pools with work stealing. Parallel command recording on explicit graphics APIs. Fibers only if waits inside jobs are pervasive | Per-platform affinity tables exist. Traces show no idle workers in the hot path | More than about five people touch threaded code, or QA finds races the tools missed |
| S4 | Whole-frame job graph with runtime data-access validation in debug builds | Validation catches a deliberately seeded race | Server work needs isolation |
| S5 | Many independent simulations per process using isolated arenas, or one process per shard. NUMA tuning. Evaluate `std::execution` once toolchains ship it | Shards cannot stall each other in load tests | — |

Guidance Inference the report's synthesis

**Build vs buy.** Rent the scheduler (enkiTS, Taskflow or oneTBB), always behind an engine API; marl's archival shows why. Build the API, the ECS scheduling, the command buffers and the validation.

**Lock early.** The job API shape. Also the rule that the audio callback thread is never a job worker and never waits on a counter.

**Pitfalls.**

- An MPMC queue where producer order matters.
- Hard affinity on hybrid CPUs.
- Expecting multi-threaded OpenGL to help.
- Mutexes held across fiber waits.
- Relying on TSan for Windows-only races.

**Learn.** The C++ memory model (acquire/release), false sharing, work-stealing deques, lock-free queue semantics, Gregory ch. 4, and the Gyrling and Genova GDC talks.

Why it matters

Thread-safety rules must exist before threads multiply, while the job system itself is the roadmap's one trigger-gated phase (P10).

[Back to the category map](#category-map)

### Core utilities

#### Math

The conventions are the contract, the library is not.

What it is

The math layer supplies vectors, matrices and quaternions, plus the conventions around them: handedness, up axis, units and depth range.

Key findings Established sourced facts, as observed in early November 2026

- **Defaults disagree across APIs and libraries.**
  - GL's clip volume defaults to a lower-left origin and −1..1 depth. `glClipControl` (GL 4.5+) can switch it ([Khronos](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glClipControl.xhtml)).
  - GLM (MIT or Happy Bunny, 1.0.3) needs `GLM_FORCE_LEFT_HANDED` and `GLM_FORCE_DEPTH_ZERO_TO_ONE` to change its defaults. Its 1.0.3 notes list "Fixed Quaternion `rotate` direction (reverted)" ([GLM](https://github.com/g-truc/glm)).
  - HandmadeMath (CC0) is column-major and ships LH/RH and ZO/NO variants side by side ([HandmadeMath](https://github.com/HandmadeMath/Handmade-Math)).
  - DirectXMath (MIT) needs a `sal.h` shim off Windows and has no `XMVECTOR` operator overloads under clang/GCC ([DirectXMath](https://github.com/microsoft/DirectXMath)).
- **Standard SIMD.** C++26 `std::simd` landed in libstdc++ with "Only x86 is supported for now" ([libstdc++-cvs](https://gcc.gnu.org/pipermail/libstdc++-cvs/2026q1/044515.html)).
- **Determinism.** Bruce Dawson writes "if you need determinism across architectures you will have to avoid fmadd," and notes that sin and cos results "are not defined by the IEEE standard" ([randomascii](https://randomascii.wordpress.com/2013/07/16/floating-point-determinism/)). Fiedler records that MotoGP could not "share saved replays between Xbox and PC" ([Gaffer On Games](https://gafferongames.com/post/floating_point_determinism/)).
- **Precision.** Jolt's floats are accurate "within roughly 5 km from the origin." A naive all-double version "has been measured to run more than 2x slower" ([Jolt Architecture](https://github.com/jrouwe/JoltPhysics/blob/master/Docs/Architecture.md)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | A math library (GLM, HandmadeMath or your own) behind engine type aliases. A one-page conventions doc (handedness, storage order, vector convention, up axis, SI units, depth range, quaternion order) encoded as unit tests | Tests such as "camera looks down −Z", "rotating X by 90° maps Y to Z" and "near plane maps to 0" pass | The first SIMD hot spot, or the first replay or desync bug |
| S2 | SoA SIMD paths for batch math (culling, skinning, particles). Simulation code compiled without fast-math. Floating-point-environment asserts once per frame on every thread | SIMD and scalar paths agree in parity tests | A second compiler or platform, or a cross-machine determinism requirement |
| S3 | Cross-compiler golden tests. FMA contraction off in simulation code. Own deterministic trig functions if the sim must match across platforms. NEON paths on ARM | Determinism CI passes wherever the design requires it | Several titles share the math |
| S4 | Stable layout for math types that reach files or packets (explicit fields, explicit endianness). A wide-SIMD batch library | No raw SIMD struct is ever serialized | Worlds beyond float precision, or a lockstep or rollback genre |
| S5 | Large-world coordinates (double or fixed-point world positions with local float frames). A fixed-point simulation core only where bit-exact results are required | Precision tests pass at the world's extents | — |

Guidance Inference the report's synthesis

**Build vs buy.** Buy the library. Build the conventions doc and its tests.

**Lock early.** Contract [#1](#contract-1). It leaks into assets, shaders, physics data and saves.

**Pitfalls.**

- A different handedness or depth range in the math library than in the graphics API.
- Minor library versions that change behavior.
- Using epsilons to "fix" determinism.
- Trig results that differ between C runtimes.

**Learn.** Linear algebra and quaternions (Gregory ch. 5), IEEE-754, the Dawson and Fiedler articles, and SIMD intrinsics.

Why it matters

The conventions are contract [#1](#contract-1) and leak into every asset, shader, physics body and save.

[Back to the category map](#category-map)

#### Containers, strings, hashing and logging

Deterministic IDs, UTF-8 everywhere.

What it is

This covers the basic data structures, string handling, hashed string IDs and logging that every other system uses.

Key findings Established sourced facts, as observed in early November 2026

- **The STL's limits.** EASTL's case: standard allocators are "painful to work with," "An empty container should allocate no memory," and debug builds heavy in std STL "can be unacceptably slow" ([N2271](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2271.html)).
- **Hash maps.**
  - Abseil's flat maps invalidate pointers on rehash and iterate in a non-deterministic order. Its guide still says "Use `absl::flat_hash_map` most of the time" ([Abseil](https://abseil.io/docs/cpp/guides/container)).
  - ankerl::unordered_dense (MIT, 5.2.0) iterates in insertion order over a contiguous vector, but its references are unstable ([unordered_dense](https://github.com/martinus/unordered_dense)).
  - `std::flat_map` first ships in GCC 15, libc++ 20 and MSVC 19.51 ([cppreference](https://en.cppreference.com/w/cpp/compiler_support/23)).
- **Hashed IDs.** EnTT's `hashed_string` uses FNV-1a, "conflicts are possible," and `type_name` "may differ depending on the compiler" ([EnTT wiki](https://github.com/skypjack/entt/wiki/Core-Functionalities)). XXH3 "produces hashes identical across all platforms," at 31.5 GB/s on an i7-9700K ([xxHash](https://github.com/Cyan4973/xxHash)).
- **Strings.** UTF-8 Everywhere treats every `std::string` as UTF-8 and converts "as close to API calls as possible" ([utf8everywhere.org](https://utf8everywhere.org/)).
- **Formatting and logging.**
  - {fmt} is "\~50% faster than printf" ([fmt](https://github.com/fmtlib/fmt)).
  - spdlog's async mode runs about 585k msg/s with the blocking policy ([spdlog](https://github.com/gabime/spdlog)).
  - Quill gives each frontend thread its own lock-free SPSC queue ([Quill](https://github.com/odygrd/quill)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Standard containers on engine memory resources. `std::format` or {fmt}. A logging library with categories. Compile-time string IDs plus a debug reverse-lookup table. UTF-8 internally, widened only at OS calls | IDs match at compile time and run time. UTF-8 paths round-trip on Windows | A hash map appears in profiles, or logging appears in frame time |
| S2 | Open-addressing maps. Small and fixed vectors. A build-time ID collision check. Low-latency async logging with a declared drop/block policy | Logging and maps allocate nothing per frame. The collision check runs in the content build | Console STL differences, or slow debug builds |
| S3 | An owned container set where the platform STL is a liability. A ban on iteration-order-dependent simulation logic | -O0 builds are usable. No sim output depends on hash-map order | Several titles and tools share IDs |
| S4 | A frozen, versioned ID spec (algorithm, width, seed, case and slash normalization). Type IDs from explicit registration, never compiler names | The spec is documented and version-tagged | Server keys arrive from untrusted clients |
| S5 | 64-bit or wider IDs across services. Seeded hashing for maps keyed by client data. Structured logs shipped off-box | Logs from every service are queryable in one place | — |

Guidance Inference the report's synthesis

**Build vs buy.** Buy the maps, formatting and logging. Build the ID scheme.

**Lock early.** The ID spec (contract [#5](#contract-5)), before content volume grows.

**Pitfalls.**

- 32-bit hash collisions as content grows.
- Hashing paths that aren't normalized.
- Iterating a hash map to produce network or replay output.
- `std::wstring` leaking past the OS boundary.
- Async log queues that silently drop errors.

**Learn.** Hashing and probing, cache behavior, allocator-aware containers, and Unicode code units vs code points vs grapheme clusters.

Why it matters

The ID and hashing scheme (contract [#5](#contract-5)) ends up in files and packets, so it must be deterministic and versioned early.

[Back to the category map](#category-map)

#### Random numbers

Own the algorithm, save the state.

What it is

Random number generation covers the generator algorithm, how it is seeded, and how separate streams are kept for separate systems.

Key findings Established sourced facts, as observed in early November 2026

- **Generators.** Vigna recommends `xoshiro256++` as the all-purpose 64-bit generator, SplitMix64 for seeding, and jump functions for "2^64 non-overlapping sequences" ([prng.di.unimi.it](https://prng.di.unimi.it/)). PCG (Apache) offers multiple streams and jump-ahead ([pcg-random.org](https://www.pcg-random.org/)).
- **Seeding.** Melissa O'Neill shows that seeding mt19937 from one 32-bit value means "it can't *ever* generate `7`, or `13` as its first output," and that `seed_seq` "is not acting as a bijection" ([pcg-random.org](https://pcg-random.org/posts/cpp-seeding-surprises.html)).
- **Portability.** Standard distributions "are not guaranteed to give the same result across different library implementations" ([aarand](https://github.com/LTLA/aarand)). WG21 paper P2059R0 makes the same point formally ([P2059R0](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p2059r0.pdf)).
- **Security.** None of these generators is cryptographically secure ([prng.di.unimi.it](https://prng.di.unimi.it/)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | One small PRNG type (PCG32, xoshiro256++ or xoroshiro128++) seeded through SplitMix64. Engine-owned integer and float range mapping. No global RNG shared across threads | The same seed gives the same sequence in a unit test | Procedural content, saves or networking |
| S2 | Named streams per system (layout, loot, AI, cosmetic VFX). The full state of every stream is saved. Content derives from hash(seed, coordinates or IDs), so generation order cannot change results | A mid-run save and load continues the identical sequence | Cross-platform builds |
| S3 | Golden tests of the first N outputs and of generated content on every compiler and OS | Golden outputs match on all shipping platforms in CI | Tools and titles share the RNG |
| S4 | Versioned algorithm IDs. Seeds logged in crash reports and replays. Cosmetic streams separated from simulation streams | Any bug report with a seed reproduces | Server authority and economies |
| S5 | Server-only RNG for authoritative outcomes. The OS CSPRNG for tokens, anti-cheat and anything gambling-like | No client-chosen randomness can create value | — |

Guidance Inference the report's synthesis

**Build vs buy.** Implement a reference generator in the engine; it is tens of lines. Never use `std::` distributions for gameplay.

**Lock early.** Contract [#9](#contract-9).

**Pitfalls.**

- Seeding mt19937 from 32 bits.
- One shared stream consumed in nondeterministic job order.
- RNG draws inside unordered-container iteration.

**Learn.** Period, equidistribution and stream independence; counter-based hashing; fixed-point range mapping.

Why it matters

Seeds become save data and shared state (contract [#9](#contract-9)), so the engine must own the algorithm and get identical output everywhere.

[Back to the category map](#category-map)

### Time and settings

#### Game loop and time

One fixed tick, two clocks.

What it is

The game loop advances the simulation in fixed time steps and renders between them. It also defines which clock each system reads.

Key findings Established sourced facts, as observed in early November 2026

- **The loop.** Fiedler's canonical loop measures frame time, clamps it (0.25 s in the article), accumulates it, consumes it in fixed steps, and renders a blend with `alpha = accumulator / dt`. Capping the number of steps prevents the "spiral of death" ([Fix Your Timestep!](https://gafferongames.com/post/fix_your_timestep/)). Nystrom notes that "most game developers I know recommend against" variable timesteps ([Game Programming Patterns](https://gameprogrammingpatterns.com/game-loop.html)).
- **Clocks.** `steady_clock` is monotonic and "most suitable for measuring intervals" ([cppreference](https://en.cppreference.com/w/cpp/chrono/steady_clock)). `SDL_DelayPrecise` waits "as close to the requested time as possible, busy waiting if necessary" ([SDL wiki](https://wiki.libsdl.org/SDL3/SDL_DelayPrecise)).
- **Windows timers.** Since Windows 10 2004, `timeBeginPeriod` "no longer affects global timer resolution." On Windows 11 the higher resolution isn't guaranteed while the window is occluded, and it can keep the CPU out of power-saving modes ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/win32/api/timeapi/nf-timeapi-timebeginperiod)). `CREATE_WAITABLE_TIMER_HIGH_RESOLUTION` (Windows 10 1803+) ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-createwaitabletimerexw)) let the Go runtime sleep "as low as about 500 microseconds" without `timeBeginPeriod` ([Go commit](https://go.googlesource.com/go/+/515e6a9b12dfe654c86cfd070ee5d6ac144fe116%5E%21/)).
- **Network time.** Unity's netcode syncs client time to the server with "small changes to the time progress rather than doing large changes" ([Unity docs](https://docs.unity3d.com/Packages/com.unity.netcode@1.5/manual/time-synchronization.html)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Fixed simulation tick (60 Hz is typical) with accumulator, clamp, substep cap and interpolated rendering. One monotonic `Clock::Now()` in int64 ticks. Simulation / Presentation / Input split. Headless flag | No gameplay code reads wall-clock time or render delta. The game runs identically with rendering off | Uncapped or no-vsync modes, laptops, frame-time spikes |
| S2 | Frame limiter: a high-resolution waitable timer for the bulk of the wait, then a short spin. No global timer-resolution hacks. Separate real time (UI, network, profiling) from game time (scaled, pausable, integer ticks) | Frame-time variance stays within budget with vsync off and a cap on. Pause and slow motion change only game time | Platforms with different present and pacing APIs |
| S3 | Per-platform pacing through swapchain present timing. Background and occluded handling. Injectable time source | A headless server runs on an injected clock in tests | Replays, rollback or networking |
| S4 | Tick numbers as canonical simulation time. A recorded-input timeline. Real-time vs sim-time APIs enforced by lint | Lint rejects wall-clock reads in simulation code | Online play at scale |
| S5 | Client-server tick sync: an RTT-based estimate, server feedback on command buffering, and slewing rather than jumps. UTC wall clock only for logs, events and economies | Ticks never run backward under jitter | — |

Guidance Inference the report's synthesis

**Build vs buy.** Build it. It is small and central.

**Lock early.** Contract [#2](#contract-2).

**Pitfalls.**

- Confusing `high_resolution_clock` with `steady_clock`.
- 1 ms sleep granularity against a 16.6 ms budget.
- Timer hacks that drain batteries.
- Clock jumps that snap networked time.

**Learn.** Fiedler and Nystrom on loops, OS schedulers and timers, RTT/2 clock estimation, and slewing.

Why it matters

The fixed tick is contract [#2](#contract-2), and replays, networking and automated tests all build on it.

[Back to the category map](#category-map)

#### Configuration and CVars

Precedence is a public interface.

What it is

Configuration is the layered set of settings files and command-line overrides. CVars (console variables) are named tunables that can be changed while the game runs.

Key findings Established sourced facts, as observed in early November 2026

- **Quake.** Quake's cvars hold scalar or string values that "can be changed or displayed at the console." An archive flag saves them on quit, and a server flag "notifies players when changed" ([Quake cvar.h](https://github.com/id-Software/Quake/blob/master/WinQuake/cvar.h)).
- **Unreal config files.** Unreal layers config across 12 levels, where "key-value assignments in latter files override those from files earlier in the hierarchy." "Command-line overrides do not work with arrays" ([Unreal config](https://dev.epicgames.com/documentation/en-us/unreal-engine/configuration-files-in-unreal-engine)).
- **Unreal console variables.** They apply by set-by priority, are read through `GetValueOnGameThread()` and `GetValueOnRenderThread()`, and take an `ECVF_Cheat` flag. `FAutoConsoleVariableRef` "bypasses multiple features (for example, thread safety, callback, sink, cheat)" ([Unreal console variables](https://dev.epicgames.com/documentation/en-us/unreal-engine/console-variables-cplusplus-in-unreal-engine)).
- **Parser.** toml++ (MIT, C++17) supports TOML v1.0.0 and "Works with or without exceptions" ([toml++](https://github.com/marzer/tomlplusplus)).
- **Separation.** Course material based on Gregory says to keep game settings separate from user settings ([Taylor COS370](https://gfx.cse.taylor.edu/courses/cos370-game//slides/02_Systems.md.html)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | One TOML or INI file plus command-line overrides. A static CVar registry (name, type, default, help, flags), read on the main thread | Every tunable is reachable from file, command line and console | Platform- or user-specific settings appear |
| S2 | Layered config (engine → game → platform → user), with writes only to the user layer. Flags: cheat, read-only, saved, requires-restart. Atomic or per-frame-snapshot reads. A versioned portable settings file kept apart from a per-device graphics file | A dump shows each key's effective value and source layer. Cheat CVars are refused in shipping builds | Multiple platforms or SKUs |
| S3 | Per-platform config layers and an inspector. Validation and range clamping. Defined array-merge semantics | Per-platform effective-config tests pass | Many contributors and titles |
| S4 | Config schema and versioning. Hot reload. Generated CVar docs. Per-title overrides of engine defaults | Docs regenerate from the registry on every build | Live service |
| S5 | Remote config and feature flags with audit and rollback. Server-authoritative CVars with a replicated subset | Ops change live tuning without a deploy and can roll it back | — |

Guidance Inference the report's synthesis

**Build vs buy.** Buy a parser. Build the registry, the layering and the flags.

**Lock early.** The precedence order and the CVar names, once players or modders edit files.

**Pitfalls.**

- Static-registration order.
- Worker threads mutating CVars mid-frame.
- Silent precedence surprises.
- Array merges.
- Code overwriting user-facing CVars.

**Learn.** Parsing, static-initialization order, observer patterns.

Why it matters

The precedence order and the CVar names become a public interface once players or modders edit files.

[Back to the category map](#category-map)

### Files

#### VFS and archives

The pack layout decides every patch.

What it is

A virtual file system (VFS) gives the engine one file API over loose files and pack archives. The pack layout is how assets are arranged inside those archives.

Key findings Established sourced facts, as observed in early November 2026

- **PhysicsFS.** PhysicsFS (zlib) overlays archives and directories into one search path. It allows a single write directory: "No file writing done through the PhysicsFS API can leave that write directory." It forbids "." and ".." ([PhysicsFS](https://icculus.org/physfs/)). Its website still lists 3.0.2 from 2019, but distributions ship 3.2.0 ([openmamba](https://src.openmamba.org/rpms/libphysfs/commit/630952b2efbbd10c4cc18abe6408b0b7064f57b8)).
- **Unreal IoStore.** IoStore packs data into `.utoc`/`.ucas` containers that load by **chunk ID** rather than path. Its offline dependency graph cut runtime dependency tracking "from seconds down to milliseconds" ([Zen loader](https://dev.epicgames.com/documentation/en-us/unreal-engine/zen-loader-in-unreal-engine)).
- **SteamPipe.** SteamPipe splits files into \~1 MB chunks, and a changed 25 GB pack makes the client build "a new, 25GB file" ([Steamworks](https://partner.steamgames.com/doc/sdk/uploading)). Valve's layout advice from the same page:
  - "Avoid shuffling asset ordering."
  - Keep packs to "one or two gigabytes."
  - Put one TOC near the start or end.
  - Compress per asset, never the whole pack.
  - Ship updates as new packs.
- **Async I/O.**
  - Linux io_uring shares submission and completion rings with the kernel and avoids aio's O_DIRECT-only limit ([kernel.dk](https://kernel.dk/io_uring.pdf)).
  - Windows IoRing needs build 22000 or later ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/win32/api/ioringapi/nf-ioringapi-buildioringreadfile)).
  - DirectStorage 1.4 adds Zstandard, with a GPU shader "initially optimized for content chunked to 256KB or smaller" ([DirectX blog](https://devblogs.microsoft.com/directx/directstorage-1-4-release-adds-support-for-zstandard/)).
- **Console storage.** SDL3 Storage exists because on consoles "game content and user data are usually two separate storage devices" ([SDL3 CategoryStorage](https://wiki.libsdl.org/SDL3/CategoryStorage)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Engine `Path` type (UTF-8, forward slashes, rooted at a content root). Every read through one engine `File` API. No absolute OS paths in game code | A grep finds no direct `fopen` or `ifstream` | Load times or file counts hurt, or a ship date is set |
| S2 | VFS with ordered mounts, read-only content mounts and one sandboxed write root. Pack v1: a TOC at the start or end, per-asset compression, 1 MB alignment, packs of at most 1–2 GB, no timestamps. Patches ship as new packs mounted later | A one-asset change produces a patch about the size of that asset. Async reads keep N requests in flight | Console ports, or streaming-heavy content |
| S3 | Platform async I/O backends (io_uring, IoRing or DirectStorage, console title storage) behind one request queue. Chunk-ID addressing at runtime | Each platform backend passes one shared I/O test suite | Mods, DLC or several titles |
| S4 | Documented overlay priority (later mount wins; explicit priority values). Containers with an offline dependency graph. Mod mounts sandboxed and treated as untrusted, with fuzzed loaders | Mount-order tests cover patch, DLC and mod overlays | Live updates and very large content |
| S5 | Content-addressed chunks delivered by CDN or on demand. Servers mount only server data | Clients fetch missing chunks on demand without a full patch | — |

Guidance Inference the report's synthesis

**Build vs buy.** PhysicsFS is optional. A custom pack reader is small, and you need control of ordering and alignment anyway. Use the platform I/O APIs directly.

**Lock early.** Path rules and the pack/TOC format, by the first public build (contract [#6](#contract-6)).

**Pitfalls.**

- Whole-pack compression or encryption, which defeats delta patching.
- Absolute offsets scattered through a pack.
- Reordering assets between builds.
- Embedded timestamps.
- Trusting paths inside archives.

**Learn.** OS file APIs, completion-ring async I/O, archive format design, and delta-patching mechanics.

Why it matters

The pack layout decides the size of every future patch (contract [#6](#contract-6)) and must be fixed by the first public build.

[Back to the category map](#category-map)

#### Compression

Choose per asset type and record the codec in the format.

What it is

Compression shrinks assets on disk and in transit, with a codec chosen per asset type.

Key findings Established sourced facts, as observed in early November 2026

- **zstd vs alternatives.** On zstd's own Silesia benchmark (i7-9700K):

  | Codec | Ratio | Compression | Decompression |
  | --- | --- | --- | --- |
  | zstd 1.5.7, level 1 | 2.896 | 510 MB/s | 1,550 MB/s |
  | zlib 1.3.1 | 2.743 | 105 MB/s | 390 MB/s |
  | lz4 1.10.0 | 2.101 | 675 MB/s | 3,850 MB/s |

  Trained dictionaries help small inputs, with gains "mostly in the first few KB" ([zstd](https://facebook.github.io/zstd/)).
- **LZ4.** Decode speed is the same at every level: "All versions feature the same decompression speed" ([LZ4](https://github.com/lz4/lz4)).
- **Oodle.** It is commercial ("Email us to get an evaluation SDK!"), with vendor claims of being "often 10× faster to decode" ([RAD](https://www.radgametools.com/oodle.htm)). It is free only inside Unreal ([Unreal blog](https://www.unrealengine.com/blog/oodle-now-free-to-use-in-unreal-engine-via-github)).
- **Pack files.** Valve says "we generally don't recommend using general compression on pack files." If you compress, compress per asset ([Steamworks](https://partner.steamgames.com/doc/sdk/uploading)).
- **GPU decompression.** DirectStorage 1.4 decompresses Zstandard on the CPU or the GPU ([DirectX blog](https://devblogs.microsoft.com/directx/directstorage-1-4-release-adds-support-for-zstandard/)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | None, or zstd at a low level | Round-trip fuzzing is clean: decompress(compress(x)) == x, and malformed input never crashes | Install size or load time matters |
| S2 | zstd per asset, with levels per content type and dictionaries for small assets. LZ4 where decode speed dominates. Never compress across asset boundaries | Load-time and patch-size budgets hold on min-spec hardware | GPU-decompression paths, or console load targets |
| S3 | Platform decompression: DirectStorage GPU Zstd with chunks of 256 KB or less; console hardware decoders under NDA. Evaluate Oodle commercially | The codec per asset is recorded in the cooked TOC, so codecs can change without a format break | Texture size dominates downloads |
| S4 | Content conditioning (texture RDO, shuffling) before entropy coding. Per-type dictionaries retrained by the build | CI refreshes dictionaries and tracks ratios | Bandwidth bills |
| S5 | Packet compression (dictionaries trained on traffic, or a commercial network codec). Compressed server storage and logs | Bandwidth per player is measured before and after | — |

Guidance Inference the report's synthesis

**Build vs buy.** Buy the codecs. Build the per-asset policy.

**Lock early.** A codec ID in every chunk header.

**Pitfalls.**

- Max-ratio levels that blow decode budgets.
- Licensing surprises, such as using Oodle outside Unreal.
- GPU-decompression chunk sizes that don't match the pack layout.

**Learn.** LZ77 and entropy-coding basics, and compression benchmarking methodology.

Why it matters

Recording the codec in each chunk lets codecs change later without breaking the pack format.

[Back to the category map](#category-map)

### Data

Overview

The data layer turns source files into cooked runtime data, and turns runtime state into saves and scenes. Both produce formats that outlive the code that wrote them.

Saves, scenes, the editor and replication should share one reflection layer, including its field IDs and flags. They should share nothing else. Fiedler's measurements show why the encoders must diverge:

- A naive 60 Hz snapshot of 901 cubes cost **17.37 Mbps**.
- Deltas against an acknowledged baseline brought a resting scene to about **15 kbps** ([Snapshot Compression](https://gafferongames.com/post/snapshot_compression/)).

A save, by contrast, must survive build changes, while both ends of a network link run the same build.

#### Asset pipeline and resources

Buy the parsers, own the cache key.

What it is

The asset pipeline converts source files (models, textures, audio) into cooked runtime data offline. The resource manager loads that data and hands out handles to it.

Key findings Established sourced facts, as observed in early November 2026

**Importers.**

- **ufbx** v0.23.1 (MIT or public domain) is a "Single source file FBX file loader". It sanitizes loaded scenes by default ("no out-of-bounds indices or non-UTF-8 strings"), and its parsers are AFL-fuzzed with 95% branch coverage ([ufbx](https://github.com/ufbx/ufbx)).
- **cgltf** v1.15 (MIT) is an stb-style glTF loader used in bgfx, Filament and raylib ([cgltf](https://github.com/jkuhlmann/cgltf)).
- **fastgltf** v1.0.0 (MIT) is C++20 and SIMD-accelerated ([fastgltf](https://github.com/spnda/fastgltf)).
- **Assimp** v6.0.5 imports "more than 40 file formats" ([Assimp](https://github.com/assimp/assimp)).

**Encoders and formats.**

- **meshoptimizer** v1.3 (MIT) covers vertex-cache, overdraw and fetch optimization, quantization, and codecs decoding at "3-6 GB/s". It also does simplification and meshlets ([meshoptimizer](https://github.com/zeux/meshoptimizer)).
- **KTX-Software**'s stable release is 4.4.2; 5.0.0 exists only as a release candidate ([KTX-Software](https://github.com/KhronosGroup/KTX-Software)).
- **Basis Universal** 2.50 supports eight codecs, but its documentation is "still ongoing" ([basis_universal](https://github.com/BinomialLLC/basis_universal)).
- **glTF 2.0** is ISO/IEC 12113:2022. It is designed to minimize "the runtime processing needed to unpack and use" assets ([Khronos glTF](https://khronos.org/gltf)).

**Reference pipelines.**

- **Godot** keeps sources in the project folder and commits a `.import` sidecar per asset. It leaves the regenerated `.godot/` folder out of version control and reimports when a source's MD5 changes ([Godot import](https://github.com/godotengine/godot-docs/blob/master/tutorials/assets_pipeline/import_process.rst)).
- **Unreal's derived-data cache** treats its contents as "disposable". A shared DDC "writes off the cost of creating any required DDC data across the entire team", and Zen is "an unauthenticated service" ([Unreal DDC](https://dev.epicgames.com/documentation/unreal-engine/using-derived-data-cache-in-unreal-engine)).

**References.**

- **Our Machinery** referenced objects by ID "so they don't break", calling path references "more fragile" ([The Truth](https://ruby0x1.github.io/machinery_blog_archive/post/the-story-behind-the-truth-designing-a-data-model/)).
- **EnTT's** resource cache is minimal and has no async loading ([EnTT](https://github.com/skypjack/entt/wiki/Resource-management)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Direct loading of glTF/PNG/WAV as a deliberate throwaway. Then an offline `cook` CLI: meshoptimizer passes and quantization, KTX2 or BCn textures, Opus or Vorbis audio. A runtime resource manager: a path-keyed cache that returns handles | Two cooks of the same input are byte-identical. The shipping runtime links no source-format parser. No duplicate loads, and shutdown leaks nothing | Load times or memory hurt, or textures need compression |
| S2 | Incremental cook. The cache key is hash(source bytes + import settings + cooker version + target platform + dependency hashes). A dependency manifest. A GUID per asset in committed sidecars; derived output is never committed. An asset "fsck" validator. Packs per level or feature. Async loads with dependency-aware states. Dev hot reload behind handles | A no-op cook takes seconds. Touching one texture recooks only it and its dependents. Renaming a source breaks no reference | A second platform |
| S3 | Platform becomes a first-class key dimension: per-platform texture formats, shader binaries and audio codecs. Per-category streaming budgets | One source tree cooks N platforms in CI | Two or more people cook, or a CI cook exceeds \~10–15 minutes |
| S4 | A shared derived-data cache (LAN or object store) populated by CI, using the same key | A fresh clone gets cooked data without cooking locally | Content reaches tens or hundreds of GB, or live patches ship on a cadence |
| S5 | Content-addressed chunk storage, distributed cooking, on-demand download, patch manifests | A live patch downloads only the changed chunks | — |

Guidance Inference the report's synthesis

**Build vs buy.** Buy every parser and encoder:

- ufbx for FBX. It is narrow, sanitized and fuzzed.
- cgltf or fastgltf for glTF.
- meshoptimizer for meshes.
- KTX-Software, pinned at 4.4.2 until 5.0 leaves release candidate.
- Opus for audio.

Build the dependency graph, cache key, GUID database and pack writer yourself. They are small, and they encode engine policy.

**Lock early.** GUID identity, never paths. Deterministic cooking.

**Pitfalls.**

- Non-deterministic cooks (hash-map order, parallel writes, timestamps) silently break both incremental builds and delta patches.
- A cache key without the cooker version.
- Committed derived data.
- "Temporary" parsers in the runtime.
- Shared caches built before a second person exists.

**Learn.** Binary layout and endianness, content addressing, GPU block compression and sRGB, glTF structure, and dirty propagation in a build graph.

Why it matters

Deterministic cooks and GUID-based asset identity underpin incremental builds, small patches and stable references.

[Back to the category map](#category-map)

#### Serialization and saves

Version from v1, write atomically, identify stably.

What it is

Serialization turns runtime state into bytes and back. Here it covers reflection (runtime descriptions of types and fields), scene files and save games.

Key findings Established sourced facts, as observed in early November 2026

**Engines select saved fields through per-field metadata.**

- **Unreal** marks fields `UPROPERTY(SaveGame)` and serializes through an archive with `ArIsSaveGame` ([Tom Looman](https://tomlooman.com/unreal-engine-cpp-save-system/)).
- **Godot's** binary path stores only `PROPERTY_USAGE_STORAGE` properties ([Godot docs](https://github.com/godotengine/godot-docs/blob/master/tutorials/io/saving_games.rst)).

**C++26 static reflection is arriving, but not evenly.**

- It was voted in June 2025 ([Sutter](https://herbsutter.com/2025/06/)).
- GCC 16.1 ships it behind `-std=c++26 -freflection` ([isocpp](https://isocpp.org/blog/2026/04/gcc-16.1)).
- Compilers spell annotation queries differently, and MSVC support was not confirmed ([wrocpp](https://wrocpp.github.io/toolset/compiler-support/)).

**Schema formats publish the same evolution rules.**

- **FlatBuffers:** add fields at the end, deprecate rather than delete, and never change defaults ([FlatBuffers](https://flatbuffers.dev/evolution/)).
- **Protobuf:** "Changing field numbers for any existing field is not safe" ([Protobuf](https://protobuf.dev/programming-guides/proto3/)).
- **Cap'n Proto:** "You cannot change a field or method parameter's type or default value" ([Cap'n Proto](https://capnproto.org/language.html)).

**Durability depends on the OS.**

- POSIX `rename()` replaces a file atomically but fails with `EXDEV` across mount points ([rename(2)](https://manpages.ubuntu.com/manpages/resolute/man2/rename.2.html)).
- `fsync()` alone does not persist the directory entry ([fsync(2)](https://manpages.ubuntu.com/manpages/resolute/man2/fsync.2.html)).
- `ReplaceFileW` documents partial-failure error codes and never promises atomicity ([Microsoft Learn](https://learn.microsoft.com/windows/win32/api/winbase/nf-winbase-replacefilew)).
- Xbox `XGameSave` states "Updates are atomic", with at most 16 MB per update ([XGameSave](https://learn.microsoft.com/en-us/gaming/gdk/docs/features/common/game-save/xgamesave)).

**Cloud saves.** Steam Cloud advises "Avoid machine-specific configurations such as video settings." It warns that enabling Dynamic Cloud Sync without handling its callbacks "could lead to data loss" ([Steamworks Cloud](https://partner.steamgames.com/doc/features/cloud)).

**ECS persistence.** EnTT's snapshot loader restores identical IDs only into an empty registry, and data must be restored "in exactly the order it was serialized" ([EnTT wiki](https://github.com/skypjack/entt/wiki/Entity-Component-System)).

**Fuzzing.** A libFuzzer target "must tolerate any kind of input" ([libFuzzer](https://llvm.org/docs/LibFuzzer.html)).

**Console rules.** Xbox requires three things ([Xbox Requirements](https://learn.microsoft.com/id-id/gaming/gdk/_content/gc/policies/console/certification-requirements)):

- updated versions load older saves (XR-003-16)
- missing add-ons never block main features (XR-037)
- saves work across console types in a generation (XR-130)

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Byte streams: explicit little-endian, a sticky error flag, length caps, and one `serialize(Stream&, T&)` for both directions. A reflection macro fills a runtime TypeInfo table with name-hash type IDs, stable field IDs and flags (`Persist`, `Replicated`, `Transient`, `EditorOnly`, `Asset`). JSON scenes with per-entity GUIDs. Atomic writes: serialize to memory → temp file in the same directory → flush → rename plus directory fsync, or `ReplaceFileW` plus a `.bak` | Any component dumps to JSON with no per-type code. Save→load→save is byte-identical. Killing the process at every write step leaves a loadable file | First public build |
| S2 | Save v1. A header (magic, format version, build hash, size, checksum). Chunks `{id, version, length, crc}`, with unknown chunks skipped and a migrator chain per chunk. Entities saved by PersistentId and loaded as a delta over the scene. Every RNG stream's state. Golden saves from every release loaded in CI. A fuzz target per loader. Frozen storage identifiers. Cloud sync that excludes device-specific settings | Golden saves from every tag load. Newer formats are rejected cleanly. A save from one OS loads identically on another | A console port |
| S3 | A storage abstraction (open → ready → read/write → close) with an "unavailable" path in the UI. Console save APIs (atomic updates, size caps). Compatibility across patches and DLC removal | Platform save tests pass: update compatibility, DLC absent, storage unavailable | Several titles and tools share formats |
| S4 | Reflection codegen, or migration to C++26 reflection, with no on-disk change. A shared schema registry for tools and titles. Migration tooling | Every schema change ships with a tested migrator and a version bump | Persistent online progression |
| S5 | Authoritative persistence on servers (a database). The network encoder shares field IDs and flags but quantizes and delta-encodes on its own | Clients never author persistent progression | — |

Guidance Inference the report's synthesis

**Build vs buy.** Build the reflection layer, byte streams, save container and migrators; they are policy. Buy JSON (nlohmann's MessagePack encoding ran 50.5–86% of minified JSON size ([nlohmann](https://json.nlohmann.me/features/binary_formats/))) and fuzzers. Schema compilers are an option for tool and network messages, and their evolution rules apply either way.

**Lock early.** Contracts [#3](#contract-3), [#4](#contract-4), [#7](#contract-7) and [#8](#contract-8).

**Pitfalls.**

- Runtime entity bits written into files.
- `std::` distributions in seeded content.
- `rename` across mount points.
- Skipping the directory fsync.
- Machine-specific settings in cloud saves.
- Retrofitting versioning onto unversioned data.

**Learn.** Byte order and alignment, schema-evolution rules, rename/fsync/`ReplaceFileW` semantics, platform storage models, fuzzing.

Why it matters

It carries four contracts ([#3](#contract-3), [#4](#contract-4), [#7](#contract-7) and [#8](#contract-8)), and a save format must be versioned from its first release.

[Back to the category map](#category-map)

## CategorySubsystems

Rendering must pick its interface first; for physics, animation, audio and video the engine buys the runtime and builds the layer that connects it to the game.

### Rendering

#### Rendering core

An explicit-style interface before the second backend.

What it is

The rendering core is the engine's interface to the GPU: the render interface, shader pipeline, materials and frame structure.

Key findings Established sourced facts, as observed in early November 2026

**Learning paths.**

- **LearnOpenGL** runs from window and triangle through shaders, textures, camera, lighting, model loading, framebuffers, instancing, shadows, HDR, deferred, SSAO and PBR. Culling and compute appear only as guest articles ([LearnOpenGL](https://learnopengl.com)).
- **vkguide** targets Vulkan 1.3 and C++20, assumes prior GL or DirectX experience, and ends with GPU-driven rendering and a framegraph ([vkguide.dev](https://vkguide.dev)).

**Production target.** Filament ships a "Clustered forward renderer"; cascaded, EVSM, PCSS and PCF shadows; TAA, FXAA and MSAA; and backend drivers kept separate from the core ([Filament](https://github.com/google/filament)).

**Every surveyed render interface looks explicit.**

- Unreal's `FDynamicRHI` is one interface with per-API implementations ([Epic](https://dev.epicgames.com/documentation/unreal-engine/API/Runtime/RHI/FDynamicRHI)).
- NVRHI defers destruction until the GPU is done and tracks resource states to place barriers. It is "more coarse-grained, more like DX12 or Vulkan" ([NVRHI](https://github.com/NVIDIAGameWorks/nvrhi/blob/main/doc/ProgrammingGuide.md)).

**Backend facts.**

- **SDL_GPU** is "a fairly thin layer" with Vulkan (Windows, Linux, Switch), D3D12 (Windows, Xbox via GDK) and Metal backends. It has no OpenGL backend, no ray tracing or mesh shaders, and four uniform slots per stage ([SDL3 GPU](https://wiki.libsdl.org/SDL3/CategoryGPU)).
- **OpenGL 4.6** (2017) added SPIR-V ingestion and indirect-parameter draws ([Khronos](https://www.khronos.org/news/press/khronos-releases-opengl-4.6-with-spir-v-support)).
- **AZDO techniques** gave "\~5x – \~15x" speedups, but only in driver-limited cases ([AZDO](https://www.slideshare.net/slideshow/gl-efficiency/32590355)).
- **macOS** stops at GL 4.1, which Apple has deprecated ([AnandTech](https://home.anandtech.com/print/12894/apple-deprecates-opengl-across-all-oses)).
- **Steam Deck:** Valve recommends "targeting Vulkan as your primary graphics API for best performance and battery life" ([Steam Deck recommendations](https://partner.steamgames.com/doc/steamdeck/recommendations)).
- **Console APIs are gated.** bgfx's GNM backend is for "licensed PS4 developers only" ([bgfx](https://bkaradzic.github.io/bgfx/overview.html)). The Forge's console platforms are "only available for accredited developers on request" ([The Forge](https://github.com/ConfettiFX/The-Forge)).

**Shaders converge on SPIR-V.**

- Slang became a Khronos initiative. Valve compiled Source 2's HLSL with it after changing 10 lines ([Khronos](https://www.khronos.org/news/press/khronos-group-launches-slang-initiative-hosting-open-source-compiler-contributed-by-nvidia)).
- DirectX will accept SPIR-V from Shader Model 7 ([DirectX blog](https://devblogs.microsoft.com/directx/directx-adopting-spir-v/)).
- On PlayStation, "shaders *must* be precompiled" ([FNA](https://fna-xna.github.io/docs/appendix/Appendix-B:-FNA-on-Consoles)).

**Render graphs.**

- Frostbite's FrameGraph cut render-target memory at 4K from **1042 MB to 472 MB**, and world-renderer code from about 15K to 5K lines ([FrameGraph](https://www.slideshare.net/DICEStudio/framegraph-extensible-rendering-architecture-in-frostbite)).
- Unreal's RDG keeps an "Immediate Mode" debug path ([Epic RDG](https://dev.epicgames.com/documentation/en-us/unreal-engine/render-dependency-graph-in-unreal-engine)).

**Tooling.** RenderDoc captures GL core 3.2–4.6 on Windows and Linux, but shader debugging covers only D3D11, D3D12 and Vulkan ([RenderDoc](https://renderdoc.org/docs/getting_started/features.html)).

| First backend | For | Against |
| --- | --- | --- |
| OpenGL 4.6 core (DSA, persistent-mapped rings, multi-draw indirect) | Fastest to first pixel. LearnOpenGL maps 1:1. RenderDoc support | No public console path. macOS stuck at 4.1. A second backend is certain later |
| SDL_GPU | One explicit API reaching Vulkan, D3D12 and Metal, including Xbox and Switch | No GL. No ray tracing, mesh shaders or bindless. PlayStation not listed |
| Vulkan 1.3 | Maximum control. Valve's recommended API on Deck. The vkguide path | Steepest curve: synchronization, memory, descriptors |
| D3D12 | Windows and Xbox | Microsoft platforms only |

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | A context with a debug callback into the engine log. A triangle behind an explicit-style interface: generational handles, immutable pipeline objects, render passes with load/store ops, explicit uploads, frame-indexed deferred destruction, binding sets, and per-pass resource usage. Offline shader compile to SPIR-V with reflection. Camera. Data-driven materials. Metallic-roughness PBR | No graphics-API enum appears outside the backend. A textured mesh is correct in sRGB. Per-pass GPU ms show in the profiler. A new material needs no new C++ | Content grows beyond a test room |
| S2 | glTF cooked into an engine format. Offscreen passes (HDR, shadows, post) that declare their inputs and outputs. Frustum culling, sort keys and instancing, then persistent-mapped rings and multi-draw indirect. Shader hot reload with a last-good fallback | The frame budget (e.g., 16.6 ms at 1080p) holds on min-spec, with per-pass timings. The shipping runtime parses no glTF. A capture justified everything beyond instancing | A second backend or platform |
| S3 | A render graph over the existing passes (derived barriers, transient aliasing, an immediate debug mode). A Vulkan or D3D12 backend with validation always on in debug. Per-platform shader cooking. Console backends under NDA | Output is identical on two backends. No hand-written barrier logic remains | Multiple titles or a dedicated rendering team |
| S4 | A shared renderer module. An artist-facing material system. Golden-image tests per backend. GPU-driven culling where captures justify it | A new title adds materials and passes without touching the backends | Open worlds, very high draw counts, or RT-quality targets |
| S5 | Compute culling and indirect draws, bindless/descriptor indexing, mesh shaders, hardware ray tracing, texture streaming | Each feature started only from a measured need | — |

Guidance Inference the report's synthesis

**Build vs buy.** Build the render interface and the frame structure. Buy the shader compilers (DXC, Slang, glslang), VMA, and the RenderDoc and Tracy integrations. Middleware renderers (bgfx, The Forge) are alternatives, but console access needs platform licensing, and The Forge also needs a commercial license.

**Lock early.** Contract [#11](#contract-11), plus the coordinate and depth conventions ([#1](#contract-1)).

**Pitfalls.**

- An interface cut at GL granularity. It turns the explicit-API port into a rewrite.
- A compatibility profile, which RenderDoc does not support.
- Treating gamma as a late topic.
- Per-shader uniforms that block batching.
- A render graph built before enough passes exist.
- Parsing glTF at runtime.

**Learn.** LearnOpenGL → vkguide → Filament's PBR document. Also std140/std430 layouts, sRGB vs linear, and explicit synchronization, memory and descriptors (Khronos [Vulkan-Guide](https://github.com/KhronosGroup/Vulkan-Guide)).

Why it matters

Its interface shape (contract [#11](#contract-11)) decides whether a second graphics backend is a port or a rewrite.

[Back to the category map](#category-map)

#### GL-class API limits

GL-class APIs reach production quality; four features need explicit APIs.

What it is

A comparison of what OpenGL 4.6-class APIs can and cannot do against Vulkan and Direct3D 12.

Key findings Established sourced facts, as observed in early November 2026

**What GL lacks.**

- **Ray tracing.** A search of the Khronos GL registry's 863 extensions finds no ray-tracing, ray-query or acceleration-structure extension ([Khronos gl.xml](https://github.com/KhronosGroup/OpenGL-Registry/blob/main/xml/gl.xml)).
- **Mesh shaders.** GL has them only as `GL_NV_mesh_shader` and `GL_EXT_mesh_shader` ([NV_mesh_shader](https://github.com/KhronosGroup/OpenGL-Registry/blob/main/extensions/NV/NV_mesh_shader.txt)).
- **Bindless.** `ARB_bindless_texture` is an extension, not core ([ARB_bindless_texture](https://github.com/KhronosGroup/OpenGL-Registry/blob/main/extensions/ARB/ARB_bindless_texture.txt)).

**What GL has.** Compute dispatch is core in GL 4.3 ([Khronos gl.xml](https://github.com/KhronosGroup/OpenGL-Registry/blob/main/xml/gl.xml)).

**Vulkan equivalents** ([vk.xml](https://github.com/KhronosGroup/Vulkan-Docs/blob/main/xml/vk.xml)):

- `VK_KHR_ray_tracing_pipeline` depends on Vulkan 1.2, `VK_KHR_spirv_1_4` and `VK_KHR_acceleration_structure`.
- `VK_EXT_mesh_shader` depends on Vulkan 1.2 and `VK_KHR_spirv_1_4`.
- Descriptor indexing is core in 1.2.

**Upscaler SDKs.** AMD's FidelityFX CAS lists DirectX 12 and Vulkan ([GPUOpen](https://gpuopen.com/fidelityfx-cas/)). FSR 4 ships as prebuilt, signed DLLs ([GamingOnLinux](https://www.gamingonlinux.com/2025/08/amd-fidelityfx-super-resolution-4-fsr-4-now-available-for-all-developers-with-fidelityfx-sdk-2-0)).

| Capability | GL 4.6 core | GL via extension | Vulkan/D3D12-class |
| --- | --- | --- | --- |
| Clustered lighting, shadow atlas, froxel fog, GTAO, TAA/SMAA, GPU particles, Hi-Z culling, CDLOD terrain, LUT sky, weighted OIT | Yes (compute, indirect draws, SSBOs) | — | Yes, plus async compute |
| Bindless textures | No | `ARB_bindless_texture` (vendor support unverified) | Descriptor indexing (core 1.2) |
| Mesh/task shaders | No | NV or EXT extensions | `VK_EXT_mesh_shader` |
| Hardware ray tracing | No | None in the registry | `VK_KHR_ray_tracing_pipeline` / `ray_query` |
| Vendor upscalers | Port MIT shaders yourself where allowed | — | Officially supported |

Guidance Inference the report's synthesis

**Rule (inference).** S1–S2 can stay GL-class with no loss for the systems in this report. S3 adds an explicit backend, which consoles need anyway. Ray tracing, mesh shaders and bindless are optional S4/S5 upgrades, each gated by a measured need. Keep probe data in an engine-owned format, so baked probes and runtime-RT probes feed one consumer.

Why it matters

It shows how long a GL-first engine can wait before it needs an explicit backend.

[Back to the category map](#category-map)

#### Lighting and shadows

Photometric units now, ray tracing later.

What it is

Lighting and shadows cover light units, how many lights a scene can shade, shadow maps, baked and dynamic global illumination, and ambient occlusion.

Key findings Established sourced facts, as observed in early November 2026

**Clustered shading.**

- Olsson, Billeter and Assarsson's clustered shading handles "two to three orders of magnitudes more lights than previously feasible."
- In a \~2,400-light scene on a GTX 480, it took **17 ms vs 26 ms** for tiled shading in total.
- Its forward variant "requires a depth pre-pass" ([Olsson et al.](https://www.cse.chalmers.se/~uffe/clustered_shading_preprint.pdf)).

**DOOM (2016) as a shipped reference.**

- DOOM ran at 60 Hz with lights and decals sharing one cluster structure ([Sousa & Geffroy](https://advances.realtimerendering.com/s2016/Siggraph2016_idTech6.pdf)).
- A frame capture reports a **16×8×24 grid** with up to 256 lights, 256 decals and 256 cubemaps per cluster ([Courrèges](https://www.adriancourreges.com/blog/2016/09/09/doom-2016-graphics-study/)).
- Shadow maps live in "a giant **8k x 8k** texture atlas" that "heavily re-uses the result of the previous frame" ([Courrèges](https://www.adriancourreges.com/blog/2016/09/09/doom-2016-graphics-study/)).

**Units and exposure (Filament).**

- Physical light units "enable full reusability of lighting rigs."
- Storing lighting in RGB16F requires pre-exposure.
- IES profiles store "luminous intensities in candela at various angles" ([Filament](https://google.github.io/filament/Filament.html)).

**Baking tools.**

- xatlas (MIT) generates unique lightmap UVs, adding vertices at seams ([xatlas](https://github.com/jpcy/xatlas)).
- lightmapper.h (public domain) bakes using "your existing OpenGL renderer" ([lightmapper](https://github.com/ands/lightmapper)).

**Dynamic GI.**

- DDGI costs **6 ms/frame at 1080p on an RTX 2080 Ti** ([Majercik et al. 2019](https://jcgt.org/published/0008/02/01/paper-lowres.pdf)).
- RTXGI requires GPU ray tracing. Its data can be baked on RT machines and loaded statically elsewhere ([RTXGI-DDGI](https://github.com/NVIDIAGameWorks/RTXGI-DDGI)).

**Ambient occlusion.** XeGTAO (MIT) costs **\~0.56 ms at 1080p on an RTX 2060** and **\~2.39 ms on Intel integrated graphics**. It relies on TAA for temporal denoising, and Intel has ceased development ([XeGTAO](https://github.com/GameTechDev/XeGTAO)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Directional, point and spot lights in a forward pass, authored in lux, lumens and candela. EV100 exposure with pre-exposure into an RGB16F target. One shadowed spot light and one shadowed point light, with PCF and per-light bias | A rig renders identically after an exposure change. No fp16 overflow or banding across a 0.1–100,000 lx test. No acne or peter-panning in a bias test scene | More shadowed lights than the budget allows, or many overlapping local lights |
| S2 | A shadow atlas with tiles per light and caching: static lights re-render only when something moves in their volume. Compute-built clustered lists for lights, decals and probes, with exponential depth slices and a depth pre-pass. Baked lightmaps (xatlas UVs plus a baker) and/or irradiance and reflection probes. GTAO-class AO | Shading cost stays roughly flat as light count rises 10×. Cached frames skip the shadow pass. The bake runs headless and reproducibly from the cook. Heat-map and atlas-occupancy views exist | A second GPU class or a console |
| S3 | Per-platform quality tiers (atlas size, PCF taps, AO preset). Async compute on explicit backends | Each tier fits its budget on its reference GPU | Several titles, or many lighting artists |
| S4 | A headless bake farm in CI. Lighting debug views. Artist light presets in photometric units. Optional DDGI baked on RT machines and shipped static | Bakes reproduce on the farm. Presets are shared across titles | Large outdoor views, or time of day |
| S5 | Stabilized or sample-distribution cascades for a sun. Streamed probe volumes. Runtime DDGI or RT shadows and reflections on RT-capable backends | GI cost fits the frame budget at the target resolution | — |

Guidance Inference the report's synthesis

**Build vs buy.** Build the clustered lists, the shadow atlas and the caching; all three are well documented. Buy xatlas and a baker, XeGTAO (MIT HLSL, now unmaintained) and RTXGI for RT backends.

**Lock early.**

- **Light units and exposure** (contract [#12](#contract-12)).
- **Forward vs deferred.** The sourced default is clustered forward with a depth pre-pass: it handles transparency and MSAA, and DOOM shipped it at 60 Hz.
- **Baked vs dynamic indirect lighting.** Use a hybrid: dynamic direct lights with shadow caching, plus baked probes or lightmaps per authored unit, in an engine-owned probe format.

**Pitfalls.**

- Unitless intensities that must be retuned later.
- Shadow bias tuned for one cascade or one light size.
- Lightmap UV seams.
- Probe light-leaking at low density.
- SSAO without TAA, which looks noisy.

**Learn.** Projective transforms and depth precision. Photometry (Filament's chapter). Compute shaders, SSBOs and atomics. UV parameterization. Hemicube or Monte Carlo integration.

Why it matters

Light units and exposure (contract [#12](#contract-12)) must be fixed before art production, or every light has to be retuned.

[Back to the category map](#category-map)

#### Atmosphere and post-processing

Lock the view transform before grading.

What it is

Atmosphere and post-processing are the full-screen steps after lighting: fog, bloom, exposure, tone mapping, anti-aliasing and color grading.

Key findings Established sourced facts, as observed in early November 2026

**Volumetric fog (Wronski, Assassin's Creed IV).**

- **Volume:** a frustum-aligned froxel texture of **160×90×64 or 160×90×128** ([Wronski](https://bartwronski.com/wp-content/uploads/2014/08/bwronski_volumetric_fog_siggraph2014.pdf)).
- **Passes:** density inject, shadowed light inject, ray-march integration, then one 3D lookup on apply that also covers transparencies.
- **Cost:** about **1.1 ms total** on that era's consoles.
- **Shadows and aliasing:** ESM shadows, and temporal jitter to remove edge artifacts.
- **Limitations:** no multiple scattering.

**Exposure and tone mapping.**

- **Filament** derives exposure from EV100, "pre-computed on the CPU" ([Filament](https://google.github.io/filament/Filament.html)).
- **Khronos PBR Neutral** reproduces base color faithfully, with no hue shifts. It is "aimed toward product photography use cases" and currently outputs sRGB only ([Khronos ToneMapping](https://github.com/KhronosGroup/ToneMapping)).
- **ACES 2.0** ships in Blender 5.0 "as a direct alternative to Filmic and AgX" ([Digital Production](https://digitalproduction.com/2025/09/02/blender-5-0-ships-built-in-aces-2-0-view-transform/)).

**DOOM's shipped post chain** ([Courrèges](https://www.adriancourreges.com/blog/2016/09/09/doom-2016-graphics-study/)):

- the Uncharted 2 tonemapper
- quarter-resolution HDR bloom
- film grain
- TAA and motion blur driven by a velocity map
- dynamic resolution scaling ([Sousa & Geffroy](https://advances.realtimerendering.com/s2016/Siggraph2016_idTech6.pdf))

**Anti-aliasing and upscaling.**

- SMAA's license does not require the notice in binary distributions ([SMAA LICENSE](https://github.com/iryoku/smaa/blob/master/LICENSE.txt)).
- FidelityFX CAS is MIT and lists DX12 and Vulkan ([GPUOpen](https://gpuopen.com/fidelityfx-cas/)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | RGB16F scene target. Manual EV100 exposure. One tonemapper. sRGB output | Output matches a reference image within tolerance. Switching tonemappers is a config flag | Art production starts, or the mood depends on fog and bloom |
| S2 | Height or exponential fog. A bloom down-sample chain at ¼ resolution. Grain and vignette. Froxel volumetric fog with temporal jitter, applied to transparents too. Motion vectors plus TAA or SMAA. Sharpening. Histogram auto-exposure. 3D LUT grading | Every pass is timed and fits its budget at the target resolution on min-spec. Fog doesn't flicker under camera motion. No ghosting on thin geometry, particles or fast cameras | An explicit-API backend, or consoles |
| S3 | Vendor upscalers through their SDKs on Vulkan/D3D12. Dynamic resolution. HDR display output as a separate output transform | Each platform holds frame rate under load with dynamic resolution | Several titles share the look pipeline |
| S4 | A shared post and LUT library. OCIO configs so DCC tools preview the in-engine look | Artists change the look without code, and DCC previews match the engine | Outdoor worlds with time of day |
| S5 | Physically based sky and aerial perspective (see [Terrain, LOD and occlusion](#terrain)). Weather- and time-driven fog density | Fog and sky stay consistent through a full day cycle | — |

Guidance Inference the report's synthesis

**Build vs buy.** Build the chain. All of it is GL-class compute. Port MIT-licensed shaders where they help. Use vendor upscalers only on the explicit backends they support.

**Lock early.** The working color space and view transform, before art production ([#12](#contract-12)). Choose PBR Neutral for fidelity, or ACES 2.0 or AgX for filmic contrast.

**Pitfalls.**

- Grading before the tonemapper and exposure are locked.
- TAA ghosting on particles that lack motion vectors.
- Fog not shadowed by the same shadow maps as surfaces.
- Froxel flicker without temporal filtering.
- Choosing PBR Neutral for a cinematic look; Khronos tunes it for product visualization.

**Learn.** Radiometry and camera exposure; participating media (extinction, in-scattering, phase functions); aliasing, reprojection and jitter; color management (OCIO, Rec. 709/sRGB).

Why it matters

The view transform, which turns scene light into display color, is part of contract [#12](#contract-12) and must be locked before grading.

[Back to the category map](#category-map)

#### Particles and VFX

CPU first, compute when counts demand it.

What it is

Particles and VFX are the systems for smoke, sparks, decals and similar effects, simulated on the CPU or the GPU.

Key findings Established sourced facts, as observed in early November 2026

**Niagara's authoring model.** Niagara builds effects from systems, emitters, modules and parameters. An emitter's stack runs Emitter Spawn, Emitter Update, Particle Spawn, Particle Update, Event Handler and Render, and "Modules are processed sequentially from top to bottom" ([Epic Niagara](https://dev.epicgames.com/documentation/en-us/unreal-engine/overview-of-niagara-effects-for-unreal-engine)).

**Hardware floor.** Unity's VFX Graph requires compute shaders and SSBOs and "does not support Open GL ES" ([Unity VFX Graph](https://docs.unity3d.com/Packages/com.unity.visualeffectgraph@17.0/manual/System-Requirements.html)).

**A GPU particle design (Wicked Engine)** ([Wicked Engine](https://wickedengine.net/2017/11/gpu-based-particle-simulation/)):

- **Buffers:** a fixed pool, a dead-index list and two alive lists.
- **Passes:** emit, simulate, an optional bitonic sort, then `DrawIndirect` of the alive count.
- **Caveat:** it warns that "Debugging a system which is living on the GPU is harder than on the CPU."

**DOOM's particles.** DOOM simulated particles in a compute shader and lit them into an atlas of "tiny fixed-size tiles" regardless of output resolution ([Courrèges](https://www.adriancourreges.com/blog/2016/09/09/doom-2016-graphics-study/)).

**Weighted blended OIT (McGuire & Bavoil)** ([McGuire & Bavoil](https://jcgt.org/published/0002/02/09/paper.pdf)):

- **Requirements:** float accumulation targets and per-target blending.
- **Weakness:** it is weakest with large depth ranges.
- **Saturation:** fp16 saturates after \~20 layers near the camera.

**Authoring tool pricing.** EmberGen's indie tier (under $1M revenue or budget) costs **$300 for the first year, then $180/yr** optional maintenance, as a permanent license ([JangaFX](https://jangafx.com/software/pricing)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | CPU emitters with a fixed pool, camera-facing quads, flipbook UVs, additive and alpha blending, and a back-to-front sort per emitter | Smoke, dust and sparks run within a fixed particle budget. Per-emitter cost shows in the profiler | Visible intersection lines, or lighting and fog mismatches |
| S2 | Soft particles (depth fade). Lit particles. Fog applied to particles. Projected decals in the clustered lists. GPU particles (compute emit/simulate, dead and alive lists, indirect draw, depth collisions) when counts demand them. A data-driven module stack with live preview | No hard intersection lines. Particles darken correctly in shadow and fog. A designer builds a new effect without engine code | Platforms with weaker GPUs |
| S3 | Compute plus SSBOs as the runtime floor, or a CPU fallback for compute-less targets. Per-platform particle budgets and LOD | Every effect holds its per-platform budget | Multiple VFX artists or titles |
| S4 | Node-graph authoring, a shared effect library, data interfaces (mesh sampling, curves, gameplay data) and an event system | Effects ship as content with no code review | Large worlds with many players |
| S5 | Effect LOD and culling by distance and significance. Cosmetic effects kept off the simulation and network paths | Effects never appear in server profiles | — |

Guidance Inference the report's synthesis

**Build vs buy.** Build the runtime. Buy flipbook authoring. Use weighted blended OIT where sorting fails.

**Lock early.** Little. The particle runtime can be swapped behind pass boundaries. The decal path follows the forward-vs-deferred choice.

**Pitfalls.**

- Sort cost exploding with emitter count.
- TAA smearing particles that have no motion vectors.
- Overdraw from large soft particles.
- WBOIT artifacts on high-contrast layers.
- Geometry-shader billboard expansion.

**Learn.** Compute, atomics, prefix sums and stream compaction, indirect draws; premultiplied alpha; depth reconstruction.

Why it matters

It is one of the few areas with little to lock, so it can start simple and be swapped later.

[Back to the category map](#category-map)

#### Terrain, LOD and occlusion

Indoor games stop at LOD and portals.

What it is

This covers how the engine limits what it draws: level-of-detail meshes, occlusion culling and, for large worlds, terrain and sky.

Key findings Established sourced facts, as observed in early November 2026

**LOD generation (meshoptimizer).**

- `meshopt_simplify` returns a normalized error usable for screen-space LOD selection.
- Attribute-aware and permissive variants exist.
- `clusterlod.h` supports "hierarchical simplification schemes similar to Nanite."
- Identical vertices must be welded, or simplification gets "stuck" ([meshoptimizer](https://github.com/zeux/meshoptimizer)).

**Occlusion culling (vkguide's Hi-Z).**

- Builds a depth pyramid from "the depth buffer from the last frame."
- Tests bounding spheres in compute and appends visible draws to indirect commands.
- Accepts that "There will be a single frame of latency on the depth pyramid" ([vkguide](https://vkguide.dev/docs/gpudriven/compute_culling/)).

**Terrain (Strugar's CDLOD).**

- A quadtree of regular grids displaced in the vertex shader.
- LOD chosen by precise 3D distance, with morphing that removes the need for stitching.
- Runs on Shader Model 3.0+ ([CDLOD](https://github.com/fstrugar/CDLOD/blob/master/cdlod_paper_latest.pdf)).

**Sky (Hillaire's atmosphere model).**

- Costs **0.31 ms at 720p on a GTX 1080**.
- Updates without the \~250 ms LUT rebuild of Bruneton's method, so time of day can change at runtime ([Hillaire 2020](https://diglib7.eg.org/bitstream/handle/10.1111/cgf14050/v39i4pp013-022.pdf)).
- The reference implementation is MIT and D3D11-only ([sebh](https://github.com/sebh/UnrealEngineSkyAtmosphere)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Hand-built levels from cooked meshes. Frustum culling | Levels load from cooked data | Triangle or draw counts exceed budget after frustum culling |
| S2 | Cook-time LOD chains selected by screen-space error. Occlusion: portals and cells or a baked PVS for interiors, or Hi-Z GPU occlusion with conservative bounds for general scenes. Instanced props with GPU culling. A static or painted sky unless exteriors matter; then a LUT sky | LOD transitions are invisible at the target resolution. Draw count drops measurably in dense interiors. Hi-Z's one-frame lag causes no popping | Explicit-API backends and weaker targets |
| S3 | Hi-Z culling as part of a GPU-driven path. Per-platform LOD bias | Each platform meets its triangle and draw budgets in the densest scene | Many levels and content teams |
| S4 | HLOD and impostor generation in the cook. LOD and culling stats tracked in CI. Visibility middleware only if in-house culling blocks several teams | CI catches LOD regressions | View distances that can't be hand-partitioned |
| S5 | CDLOD or clipmap terrain, terrain material blending and virtual texturing, HLOD streaming, water, a physical sky with time of day, cluster LOD with mesh shaders on Vulkan/D3D12-class backends | World cells stream without hitches within bounded memory. The sun moves with no LUT hitch | — |

Guidance Inference the report's synthesis

**Build vs buy.** Buy meshoptimizer. Build culling, which vkguide documents. CDLOD's source is available. Port Hillaire's MIT reference.

**Lock early.** Whether levels are authored or assembled at runtime. That choice decides whether PVS and whole-level bakes are possible at all (see [Procedural generation](#procgen)).

**Pitfalls.**

- Unwelded meshes.
- LOD error not scaled to world units.
- Hi-Z false culling from stale depth.
- A baked PVS invalidated by runtime-assembled levels.

**Learn.** BVHs, quadtrees and portal graphs; screen-space error; compute plus indirect draw; Rayleigh and Mie scattering.

Why it matters

It separates what indoor games need (LOD and portals) from open-world features that can wait until stage 5.

[Back to the category map](#category-map)

### Motion

#### Physics and collision

Integrate a library, own the character controller.

What it is

Physics and collision simulate rigid bodies, answer queries such as raycasts, and move the player through a character controller.

Key findings Established sourced facts, as observed in early November 2026

**Jolt as the 3D default.**

- Jolt (MIT, **v5.6.0**, tagged July 11, 2026) is "Used by Horizon Forbidden West and Death Stranding 2" ([Jolt README](https://github.com/jrouwe/JoltPhysics)).
- It needs Visual Studio 2022, Clang 16 or GCC 12, and C++17. It uses no RTTI and no exceptions.
- It supports "Platform Blue (a popular game console)" through an NDA'd header ([Jolt README](https://github.com/jrouwe/JoltPhysics)).
- Godot built it in as of 4.4, and it is the default for new projects ([Godot docs](https://github.com/godotengine/godot-docs/blob/master/tutorials/physics/using_jolt_physics.rst)).

**What Jolt asks of the host engine** ([Jolt HelloWorld](https://github.com/jrouwe/JoltPhysics/blob/master/HelloWorld/HelloWorld.cpp)):

- A job system.
- A temp allocator.
- Fixed capacities at init ("in the order of 65536" bodies for a real project).

**Determinism and stepping (Jolt)** ([Jolt Architecture](https://github.com/jrouwe/JoltPhysics/blob/master/Docs/Architecture.md)):

- The simulation is deterministic when APIs are called "in exactly the same order" by "The same binary code."
- `CROSS_PLATFORM_DETERMINISTIC` makes it "approximately 8% slower", and broadphase queries "are NOT deterministic."
- Rollback uses `SaveState`/`RestoreState`, but only if bodies are created in the same order.
- It is "stable when running at 60 Hz with 1 collision step."

**Alternatives.**

- **PhysX 5.12.0.** The SDK moved to Apache-2.0 at 5.11, but the repo-root README still shows BSD-3. Its public runtime targets only Windows and Linux ([PhysX CHANGELOG](https://github.com/NVIDIA-Omniverse/PhysX/blob/main/physx/CHANGELOG.md)).
- **Box2D v3.2** (October 18, 2026) adds experimental recording, replay and world snapshots ([Box2D v3.2](https://github.com/erincatto/box2d/blob/main/docs/release_notes_v320.md)).
- **Box3D.** Erin Catto still considers it "alpha software" ([Announcing Box3D](https://box2d.org/posts/2026/06/announcing-box3d/)).
- **Bullet.** Its last tag is 3.25 (2022), and its issue tracker is closed ([Bullet](https://github.com/bulletphysics/bullet3)).

**The cost of building your own.** Jolt began "in 2014 in his spare time" and covered everything Horizon used by May 2021. Broadphase waiting fell from about 500 µs to 10 µs per thread ([Rouwe GDC 2022 notes](https://jrouwe.nl/architectingjolt/ArchitectingJoltPhysics_Rouwe_Jorrit_Notes.pdf)).

**Character controllers.**

- Jolt's rigid-body `Character` "is usually not the best solution for a player." `CharacterVirtual` uses collision detection only, with stair walking and floor sticking, and swaps shapes behind a penetration check, which is how crouching works ([Jolt Architecture](https://github.com/jrouwe/JoltPhysics/blob/master/Docs/Architecture.md)).
- PhysX's kinematic controller uses "collide and slide." Its docs call a capsule "the recommended default choice" and say pushing dynamics through contact forces is "often not a very convincing solution" ([PhysX CCT](https://nvidia-omniverse.github.io/PhysX/physx/5.6.1/docs/CharacterControllers.html)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | An integrated library with its sample job pool and temp allocator. Static and dynamic layers. Raycasts. The library's kinematic character with defaults. Sensors as triggers. Debug draw of every collider | Every collider in a test level is visible. Movement works on stairs and slopes with the defaults | Gameplay code starts including library headers |
| S2 | An engine physics API: handles; a layer matrix and material table as data; `Raycast`/`Sweep`/`Overlap` returning entity handles. Engine jobs and allocators wired in. Capacities set from measured content. Tuned stairs, slopes, crouch and a door policy. CCD for fast objects. Coplanar static pieces merged to avoid ghost collisions. Contact events buffered per thread and sorted before gameplay sees them. Death ragdolls. Same-binary determinism used for replay regression tests | Gameplay never includes library headers. A capsule walks a "torture" level of stairs, ramps and doorways without sticking. A bullet-speed sphere never tunnels through a 1 cm wall. A box slides over tiled floors without hopping | A second platform or a console |
| S3 | Console builds under NDA. Pinned compilers. Cross-platform determinism only if replays or lockstep cross platforms. Per-platform budgets for bodies, active ragdolls and queries | Each platform meets its physics budget in the heaviest scene | Several programmers or titles share physics |
| S4 | A vendored, pinned library with a local patch queue and scheduled upgrades. A CI determinism job per platform and compiler. Editor UI for layers and materials | Upgrades land only with release-note review and green determinism jobs | Shared worlds or high player counts |
| S5 | Several physics systems per zone or shard. Double precision beyond about 5 km. Headless server instances. Server-authoritative snapshots for props. Rollback only for a few predicted bodies. Consider forking the library | Per-zone server physics holds tick rate at the target population | — |

Guidance Inference the report's synthesis

**Build vs buy.**

- **Jolt** is the 3D default. It is MIT-licensed, active and AAA-shipped, documents its determinism, runs on host-provided jobs and allocators, and has both character styles.
- **PhysX 5** works if Windows and Linux are enough.
- **Box2D v3** covers 2D.
- **Building in-house** is sensible only to learn (Box2D-Lite is the template), for a collision-only query layer, or at S5 for a measured blocker the library can't fix. The in-house order runs primitives → static queries → broadphase → narrowphase and manifolds → sequential impulses → islands and sleeping → joints → CCD → threading.

**Lock early.** SI units and scale, the engine physics API and its handle mapping, and the determinism promise ([#10](#contract-10)).

**Pitfalls.**

- Units outside the library's ranges.
- Touching bodies during `Update`, or trusting callback order.
- Adding bodies one at a time while streaming.
- Static sensors losing bodies that fall asleep.
- Ghost collisions on tiled floors.
- Rotational tunneling despite linear-cast CCD.
- Standing up from a crouch without an overlap check.
- Silent behavior changes in library upgrades.

**Learn.** In order:

1. Quaternions and transform hierarchies.
2. Numerical integration and rigid-body dynamics ([Baraff](https://www.cs.cmu.edu/~baraff/sigcourse/)).
3. Constraints and sequential impulses ([Catto's GDC talks](https://box2d.org/publications/)).
4. Collision detection ([Ericson](https://realtimecollisiondetection.net/books/rtcd/)).
5. Soft constraints and sub-stepping.
6. Floating-point determinism.

Why it matters

Units, the engine physics API and the determinism promise (contract [#10](#contract-10)) are fixed here and constrain replays and networking.

[Back to the category map](#category-map)

#### Animation

Buy the runtime, build the graph.

What it is

Animation plays and blends skeletal clips, drives state machines and inverse kinematics, and hands poses to the renderer.

Key findings Established sourced facts, as observed in early November 2026

**ozz-animation** (MIT, 0.17.0, August 2026) ([ozz features](https://guillaumeblanc.github.io/ozz-animation/documentation/features/)):

- Depends only on C++17.
- Provides sampling, N-way blending, masks, additive layers, skinning matrices, user tracks, root-motion extraction, and two-bone and aim IK.
- Lists retargeting and dual-quaternion skinning as planned only.

**ACL** (MIT, v2.1.0) ([ACL](https://github.com/nfrechette/acl); [Frechette](https://nfrechette.github.io/2023/09/17/acl_in_ue)):

- States "Decompression speed will not be sacrificed for a smaller memory footprint nor will accuracy be compromised."
- Has been Unreal's default animation codec since UE 5.3, with a 30%+ memory reduction.
- Its README's "UE 5.13" is a typo for 5.3.

**glTF 2.0** ([glTF spec](https://github.com/KhronosGroup/glTF/blob/main/specification/2.0/Specification.adoc)):

- Defines linear blend skinning with up to four joints per attribute pair.
- States "glTF 2.0 defines only storage of animation keyframes, so this specification doesn't define any runtime behavior".

**Dual-quaternion skinning.** Upgrading from linear blend skinning is "very easy" with a "relatively minor impact on run-time performance" ([Kavan et al. 2008](https://users.cs.utah.edu/~ladislav/kavan08geometric/kavan08geometric.html)).

**Animation graphs.**

- Godot's AnimationTree offers filtered Blend2/Blend3 nodes, Delaunay-triangulated 2D blend spaces, Immediate/Sync/At End transitions, A\*-based `travel()`, and a root-motion track ([Godot AnimationTree](https://github.com/godotengine/godot-docs/blob/master/tutorials/animation/animation_tree.rst)).
- Unreal's transitions are one-directional, with conduits and aliases for many-to-many cases ([Unreal state machines](https://dev.epicgames.com/documentation/en-us/unreal-engine/state-machines-in-unreal-engine)).
- Motion matching searches a database for the frame that best matches the current pose and desired future path ([Clavet, GDC 2016](https://gdcvault.com/play/1023280/Motion-Matching-and-The-Road)).

**IK and ragdolls.**

- FABRIK is the standard iterative IK solver for longer chains ([Aristidou](https://www.andreasaristidou.com/FABRIK)).
- Jolt supports hard keying, soft keying, motors driving to a pose, and skeleton mapping for ragdolls ([Jolt README](https://github.com/jrouwe/JoltPhysics)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | glTF/FBX import (cgltf, ufbx) into skeleton, bind pose, inverse bind matrices and clips. GPU linear blend skinning (4 influences) from a matrix palette. Sampling with lerp/slerp, looping and playback rate. Crossfade. Code-driven state switches | Khronos sample models import with the correct bind pose. A clip plays identically at 30, 60 and 144 fps render rates. Walk→run crossfades without pops | More than a few characters, or animators need to iterate |
| S2 | Offline cook to a runtime format with keyframe reduction. Masked and additive layers. 1D/2D blend spaces with phase sync. A data-driven state machine with typed parameters written in the sim tick. Root motion fed to the character controller. Edge-triggered events. Two-bone and aim IK with foot placement. Death ragdolls blended in and out | The locomotion graph is authored in data. Footstep events fire once per contact at any rate. Feet plant on 20 cm steps and 30° slopes. No pops entering or leaving ragdoll | Animation memory over budget, or a second platform |
| S3 | ACL-class compression per platform budget, with error metrics. SIMD and job-parallel sampling. Animation LOD | Memory per clip meets its budget within a set error threshold | Several skeletons share clips, or animators are blocked on code |
| S4 | A visual graph editor with live debugging (current state, weights). Retargeting. Shared clip libraries. Powered ragdolls. FABRIK for long chains | Animators ship a graph change without a programmer. One clip set drives two rigs acceptably | Crowds, or server-side hitboxes |
| S5 | Crowd LOD and shared poses. Servers evaluate only root motion and hitbox poses. Replicate graph parameters, not poses. Motion matching if there is a mocap pipeline and a dedicated programmer | Server animation cost per added player stays flat | — |

Guidance Inference the report's synthesis

**Build vs buy.** Buy the importers, and optionally the ozz runtime. Add ACL when memory bites. Build the graph, events, ragdoll bridge and GPU skinning path.

**Lock early.** Graphs are data assets from S2, even before an editor exists. Gameplay-relevant animation (root motion, events) advances on simulation time.

**Pitfalls.**

- Assuming glTF defines looping or playback.
- Blending a track that one clip lacks, with no defined rest value.
- NaNs in two-bone IK when softening targets (fixed in ozz 0.17.0).
- Ragdoll self-collision jitter without a group filter.

**Learn.** Slerp sign handling, local-to-model transforms, linear blend skinning then DQS, analytic two-bone IK then FABRIK, and state-machine design from the Godot and Unreal docs.

Why it matters

Gameplay-relevant animation (root motion, events) must run on simulation time, which ties it to the fixed tick.

[Back to the category map](#category-map)

### Media

#### Audio

A real-time threading problem before it is a sound problem.

What it is

Audio mixes and plays sound on a dedicated real-time thread. It also defines the gameplay noise events that AI can hear.

Key findings Established sourced facts, as observed in early November 2026

**SDL3 stops at the device.** SDL3 gives devices and streams, and opening the default device lets playback follow headphone changes. Its only loader is WAV. It has no mixer, voices, buses, 3D or streaming ([SDL3 CategoryAudio](https://wiki.libsdl.org/SDL3/CategoryAudio)).

**Library options.**

- **miniaudio** is "public domain or MIT No Attribution." It ships a node graph, async streaming, attenuation and doppler. "HRTF and environmental occlusion are not currently supported" ([miniaudio](https://github.com/mackron/miniaudio)).
- **OpenAL Soft** is LGPL, with HRTF and EFX effects ([OpenAL Soft](https://github.com/kcat/openal-soft)).
- **Wwise Indie** is free under a $250K production budget; Pro starts at $8,000 ([Audiokinetic](https://www.audiokinetic.com/en/pricing)).
- **FMOD's** current terms could not be read from its own page. The only citation found, from 2017, gives an indie threshold of under US$600k ([Wikipedia](https://en.wikipedia.org/wiki/FMOD)).
- **Steam Audio 4.8.1** is Apache-2.0, with HRTF, occlusion, reflections and pathing ([Steam Audio](https://github.com/ValveSoftware/steam-audio)).

**Real-time rules.**

- The audio callback must finish within its buffer period. That means no locks, no malloc, no file I/O and no logging. Commands travel through "lock-free, single-reader single-writer FIFO queues" ([Bencina](http://www.rossbencina.com/code/real-time-audio-programming-101-time-waits-for-nothing)).
- Use no `std::mutex` on the audio thread, "not even with `try_lock`" ([Doumler](https://timur.audio/using-locks-in-real-time-audio-processing-safely)).
- Stopping a miniaudio device from inside its own callback "will result in a deadlock" ([miniaudio manual](https://miniaud.io/docs/manual/index.html)).

**Buffer sizes and codecs.**

- A 256-sample buffer at 44.1 kHz leaves about **5.8 ms** per callback ([Bencina](http://www.rossbencina.com/code/real-time-audio-programming-101-time-waits-for-nothing)).
- SDL's buffer-size request "isn't guaranteed" ([SDL hint](https://wiki.libsdl.org/SDL3/SDL_HINT_AUDIO_DEVICE_SAMPLE_FRAMES)).
- Opus is natively 48 kHz ([opus-codec.org](https://opus-codec.org/)).

**AI hearing stays outside the mixer.**

- Unreal models AI hearing as a gameplay call, `ReportNoiseEvent`, with no link to playback ([UE](https://docs.unrealengine.com/4.27/en-US/BlueprintAPI/AI/Perception/ReportNoiseEvent/index.html)).
- Audiokinetic advice: "Try to avoid inversion of responsibility from the game to a piece of middleware" ([Audiokinetic Q&A](https://audiokinetic.com/qa/6164/virtual-voices-bad-practice?show=6174)).

**Production practice.**

- A standard virtualization policy is "Kill if finite else virtual" ([Taylor & Francis](https://www.taylorfrancis.com/books/9781315157542/chapters/10.1201/b22247-9)).
- *Game Audio Programming 5* (2025) covers automated testing ([Routledge](https://www.routledge.com/Game-Audio-Programming-5-Principles-and-Practices/Somberg/p/book/9781032852850)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | A device stream (float32, 48 kHz) feeding an engine mixer, or a library engine behind an engine audio facade. One-shot playback from data decoded at load time. Preallocated voices with gain and pan ramps and a master limiter. An SPSC command queue from the game thread to the audio thread | Zero underruns with hundreds of play requests. The callback takes no locks, allocates nothing and does no I/O | Music, ambience, and many simultaneous sounds |
| S2 | Streaming: a decode thread and ring buffer per stream, Vorbis or Opus, loop points, crossfades. Voice management: generational handles, per-sound caps, priority × audibility, virtual voices. A bus tree with LPF/HPF, a reverb send and ducking, swapped via immutable snapshots. 3D attenuation and panning, interpolated per block. Smoothed raycast occlusion and per-room reverb. A gameplay noise-event system separate from playback. A debug overlay for voices, buses and streaming | The starvation counter stays at zero under disk load. Sliders and filters change without clicks. AI behaves identically with audio muted or on a headless server | A console port, or a dedicated sound designer |
| S3 | Console audio backends: middleware with vendor console support, or a custom device layer under NDA. Per-platform codecs. Optional HRTF, limited to the N nearest sources | The same sound events play on every target within CPU budget | Sound designers need to author without programmers |
| S4 | Data-driven sound events (randomized containers, parameters) authored in middleware or an in-house tool. Hot reload of sound definitions. Automated audio tests | A designer adds or retunes a sound without code | Server fleets and voice chat |
| S5 | No audio code on servers, only noise events. Voice chat through platform or online services. "Max within radius" virtualization | Headless servers link no audio-device code | — |

Guidance Inference the report's synthesis

**Build vs buy.**

- **Cost is not the deciding factor.** Every option above is effectively free at small scale, so choose on workflow and console reach.
- **Fastest path.** A library engine behind a facade; a custom mixer can replace it later behind the same facade.
- **Wwise or FMOD** buy designer tooling and console support. Verify their current terms first.
- **Steam Audio** adds HRTF and occlusion.

**Lock early.** Audio-thread ownership with command queues. Noise events that don't depend on the mixer.

**Pitfalls.**

- One OS stream per voice, which leaves nowhere to put buses.
- Locks or allocation in the callback.
- Stopping the device from inside its callback.
- Un-ramped gain changes, which click.
- AI that taps the mixer.
- Middleware that pins itself to P-cores ([Intel](https://www.intel.com/content/www/us/en/developer/articles/guide/12th-gen-intel-core-processor-gamedev-guide.html)).

**Learn.**

- Sample rate, frames and decibels, and why summing clips.
- Atomics and SPSC rings.
- Panning laws, ramps, biquads, delay lines, reverb and limiters.
- Codecs.
- HRTF.
- The *Game Audio Programming* series.

Why it matters

Its threading rules are strict from the first day, and keeping noise events separate from playback lets servers run without audio.

[Back to the category map](#category-map)

#### Video playback

The license route matters more than the decoder.

What it is

Video playback decodes pre-rendered video for cutscenes and in-world screens.

Key findings Established sourced facts, as observed in early November 2026

**FFmpeg licensing.** FFmpeg is LGPL 2.1+ by default, but "If those parts get used the GPL applies to all of FFmpeg" ([FFmpeg legal](https://ffmpeg.org/legal.html)). Its own LGPL checklist:

- Compile without `--enable-gpl` and `--enable-nonfree`.
- Link dynamically.
- Distribute the matching source.
- Credit FFmpeg in the About box and EULA.
- Remove reverse-engineering bans from the EULA.

On patents, the project says: "We do not know, we are not lawyers." The latest release is 9.0.2 (September 18, 2026) ([FFmpeg download](https://ffmpeg.org/download.html)).

**Alternative decoders.**

- **dav1d** (BSD-2) is a production-ready AV1 decoder ([dav1d](https://github.com/videolan/dav1d)).
- **pl_mpeg** (MIT, single file) decodes MPEG-1 video with MP2 audio. "All patents related to MPEG1 and MP2 have expired." It decodes 4K60 on one core, but it ignores timestamps and has "no error reporting" ([pl_mpeg](https://github.com/phoboslab/pl_mpeg)).
- **libtheora 1.2.0** shipped in March 2025 ([Phoronix](https://www.phoronix.com/news/Theora-1.2-Released)).
- **Bink 2** became Epic's when it acquired RAD on January 7, 2021. Licenses "will continue to be available to those who do not use the Unreal Engine," and it supports current consoles (secondary source: [Wikipedia](https://en.wikipedia.org/wiki/Bink_Video)).

**Accessibility.** Xbox's subtitle guideline asks that captions persist in screen recordings ([XAG 104](https://learn.microsoft.com/en-us/gaming/accessibility/xbox-accessibility-guidelines/104)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | None, or pl_mpeg decoding to a texture for in-world screens | A clip plays on a mesh | Real cutscenes with audio sync and subtitles |
| S2 | A patent-safe decoder (pl_mpeg, libtheora, or dav1d with a demuxer), or FFmpeg under its LGPL checklist. Audio routed through the engine mixer. The video clock slaved to the audio clock. Captions rendered into the frame. Skippable cutscenes. Decoding off the main thread | No perceptible A/V drift over a full cutscene. Captions show. The license checklist is complete | Consoles, 4K/HDR, or file-size pressure |
| S3 | A commercial console-proven codec, or platform hardware decoders, behind one `VideoSource` interface | The same assets play on every target | Multiple titles |
| S4 | Encode presets in the cook. Per-language audio tracks and subtitle files. Streaming from packs | Artists drop in masters, and the build produces per-platform encodes | Live ops |
| S5 | CDN-streamed news and promo video in menus | Menu video updates without a client patch | — |

Guidance Inference the report's synthesis

**Build vs buy.** Buy the decoder.

**Lock early.** The codec and the license route, chosen with the console plan in mind. FFmpeg's LGPL route forces dynamic linking. Permissive decoders keep static linking open.

**Pitfalls.**

- pl_mpeg's ignored timestamps.
- GPL contamination, for example from libx264.
- Patent exposure on MPEG-LA codecs.
- Decoding on the main thread.

**Learn.** Containers vs codecs, A/V sync, and LGPL obligations.

Why it matters

The codec's license route constrains linking and console plans more than its decoder does.

[Back to the category map](#category-map)

## CategoryGameplay

In the gameplay layer the durable decisions are boundaries between systems, not algorithms: who owns transforms, when events flush, what an agent may know and what scripts can touch.

Overview

Gregory's gameplay foundation layer sits above the renderer and peer subsystems: object model, events, scripting and AI ([UMD CMSC425](https://www.cs.umd.edu/class/spring2018/cmsc425/Lects/lect02-architecture.pdf)). In this layer the durable decisions are boundaries, not algorithms:

- who owns transforms
- when events flush
- what an AI agent is allowed to know
- what script code can touch

The **fixed-tick seam rule** applies throughout:

- **Simulation tick.** Anything gameplay reads is computed here: AI, perception, navigation queries, gameplay transforms, gameplay events.
- **Presentation.** Anything that is only seen is computed here: cameras, shake, cosmetic events, interpolated transforms.

That keeps a headless server identical to a client, minus presentation.

### Logic

#### Entity and object model

A seam first, a library second.

What it is

The entity and object model is how game objects and their data are represented, either as objects or as an entity component system (ECS).

Key findings Established sourced facts, as observed in early November 2026

The case for data-oriented design:

- Mike Acton: "The transformation of data is the only purpose of any program" ([isocpp.org](https://isocpp.org/blog/2015/01/cppcon-2014-data-oriented-design-and-c-mike-acton)).
- Richard Fabian warns that "A common misconception about data-oriented design is that it's all about cache misses" ([DOD book](https://www.dataorienteddesign.com/dodbook/node2.html)).
- Contiguous arrays "can make code run anywhere from 2–16x faster" ([Mertens](https://ajmmertens.medium.com/building-an-ecs-2-archetypes-and-vectorization-fe21690805f9)).
- EnTT's author names "the code organization and not (only) the performance" as the main reason to adopt ECS ([Caini](https://skypjack.github.io/2019-02-14-ecs-baf-part-1/)).

Storage models:

- Archetype tables move the whole entity on every component add or remove, which Unity calls "resource-intensive" ([Unity Entities](https://docs.unity3d.com/Packages/com.unity.entities@1.3/manual/concepts-archetypes.html)).
- Sparse sets make add and remove cheap ([ECS FAQ](https://github.com/SanderMertens/ecs-faq)).

EnTT specifics:

- v4 requires C++20 and removed deprecated APIs ([EnTT releases](https://github.com/skypjack/entt/releases)). The repository HEAD reports 4.1.0 ([version.h](https://github.com/skypjack/entt/blob/HEAD/src/entt/config/version.h)).
- Pointer-stable storage and owning groups "refuse to compile" together, and the registry is not thread-safe ([EnTT wiki](https://github.com/skypjack/entt/wiki/Entity-Component-System)).
- Flecs, by contrast, stages structural changes per thread ([Flecs](https://www.flecs.dev/flecs/Systems.html)).

Migrating a shipped game:

- A March 2026 engineering guide gives five steps: stand up an ECS core, bridge legacy objects as components, port hot components gradually, expose the ECS to scripting, and monitor performance.
- It warns "Legacy code expects stable pointers" and concludes that "a fully ECS-based architecture is rarely achievable in existing projects" ([MCV/DEVELOP](https://mcvuk.com/business-news/guest-post-integrating-ecs-into-existing-mass-market-games-a-practical-engineering-guide)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | An engine Entity API (frozen handle type, create/destroy, add/get/remove, query/each) over a simple object model or an ECS. Pure-data components | Gameplay compiles against the API only. No library type names in game code | Per-object update cost, or code-organization pain as entity types multiply |
| S2 | An ECS library behind the API: sparse-set for cheap add/remove, or archetype for fast iteration. Systems over views. Owning groups or archetype tuning only for profiled hot loops. A PersistentId separate from runtime IDs. Lifecycle signals feed derived indices | Each converted system runs at or below its old frame time. Saves round-trip through PersistentIds | The job system arrives |
| S3 | Systems declare read/write sets. Per-thread command buffers flush at sync points, with a deterministic merge | Parallel and serial runs produce identical state | Several titles, or rotating contributors |
| S4 | A documented end state: full ECS, or a hybrid recorded in an architecture decision record. Optionally an in-house store behind the same API, A/B-benchmarked against the library. ECS memory and debug views | One shared test suite passes on both storage backends | Sharded servers |
| S5 | One world per shard or zone. Push-model change tracking that feeds replication | Replication consumes change sets without polling components | — |

Guidance Inference the report's synthesis

**Build vs buy.** Buy EnTT (MIT) or flecs. Build the seam and the persistence.

**Lock early.** The Entity API seam and stable IDs ([#3](#contract-3)). Never return library view types or signal signatures through the API.

**Pitfalls.**

- Leaking library types into gameplay code.
- Storing raw entity bits.
- Promising a full migration.
- Frequent archetype moves for tag components.

**Learn.** Acton and Fabian on DOD, the ECS FAQ on storage models, the EnTT wiki, and Mertens' "Building an ECS" series.

Why it matters

The Entity API seam and stable IDs (contract [#3](#contract-3)) let the storage change without rewriting gameplay or breaking saves.

[Back to the category map](#category-map)

#### Events and messaging

The flush contract is the design.

What it is

Events and messaging are how systems notify each other: direct calls, signals, queues and commands.

Key findings Established sourced facts, as observed in early November 2026

**Nystrom's three patterns.**

- **Observer.**
  - "A slow observer can block a subject."
  - The lapsed-listener problem is "such a common issue in notification systems that it has a name."
  - "If you often need to think about both sides of some communication in order to understand a part of the program, don't use the Observer pattern" ([Observer](https://gameprogrammingpatterns.com/observer.html)).
- **Event queue.**
  - It decouples sender and receiver "both *statically* and *in time*."
  - It warns that "The state of the world can change under you."
  - It advises avoiding "*sending* events from within code that's *handling* one" ([Event Queue](https://gameprogrammingpatterns.com/event-queue.html)).
- **Command.** Commands let player, AI and script drive actors interchangeably, and serializable command streams enable replay and network sends ([Command](https://gameprogrammingpatterns.com/command.html)).

**EnTT's primitives.**

- Immediate `trigger`: "Listeners are invoked immediately, order of execution is not guaranteed."
- `enqueue`/`update` dispatches "once per tick."
- "connecting listeners within event handlers can result in undefined behavior" ([EnTT signals](https://github.com/skypjack/entt/blob/HEAD/docs/md/signal.md)).

**Other engines.**

- Bevy 0.20's `Messages<T>` keeps "the messages that occurred within the last two `Messages::update` calls," with the update scheduled "after `FixedUpdate` systems" ([docs.rs](https://docs.rs/bevy/latest/bevy/ecs/message/index.html)).
- Unreal behavior trees are "event-driven to avoid doing unnecessary work every frame" ([UE 5.8 BT](https://dev.epicgames.com/documentation/en-us/unreal-engine/behavior-tree-in-unreal-engine---overview)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Direct calls inside a feature. Signals with RAII disconnect across module boundaries. One queue from gameplay to presentation (sound, VFX) | No raw listener pointers. Destroying a listener disconnects it (tested under ASan) | The first lapsed-listener crash, or a "who handled this?" bug |
| S2 | Typed queues that flush at documented points in the fixed tick (after input, after simulation systems, before presentation). Handlers may not connect or disconnect mid-dispatch. Events raised in handlers go to the next flush. Command objects for input, AI and replay. Component lifecycle signals drive derived state | The same inputs produce the same event order across runs, and a replay test passes. A max-iterations assert bounds feedback loops | Replays or desyncs reveal order dependence |
| S3 | Per-thread event buffers merged deterministically at sync points | Parallel and single-threaded runs produce the same merged order | New contributors break ordering assumptions |
| S4 | Event tracing (a log or an ImGui timeline). A written event-ordering contract. Event types versioned with reflection. Separate channels for cosmetic events and authoritative gameplay events | Any gameplay event can be traced from producer to consumers in a debug build | Broadcast fan-out costs bandwidth or CPU |
| S5 | Interest-managed delivery: events go only to relevant clients or shards. Strict authoritative/cosmetic split | Clients never act on authority-only events | — |

Guidance Inference the report's synthesis

**Build vs buy.** Use EnTT's dispatcher or an in-house equivalent. The contract matters more than the container.

**Lock early.** Contract [#15](#contract-15).

**Pitfalls.**

- Immediate dispatch with unspecified order inside the simulation.
- Events raised from handlers.
- A global central queue.
- Payloads that point at state that may change before handling.

**Learn.** Nystrom's Observer, Event Queue and Command chapters, and the EnTT signal docs.

Why it matters

The moment queued events flush (contract [#15](#contract-15)) determines whether replays and networked play stay in order.

[Back to the category map](#category-map)

#### Scripting

Native hot reload first, a VM only when someone needs one.

What it is

Scripting is how gameplay logic is authored and reloaded: native C++ with hot reload, or an embedded language running in a virtual machine.

Key findings Established sourced facts, as observed in early November 2026

**Language and runtime status (late 2026).**

- **Lua 5.5** shipped in December 2025, and 5.5.1 is current. "There will be no further releases of Lua 5.4" ([lua.org](https://www.lua.org/versions.html)).
- **LuaJIT 2.1** is MIT-licensed, with copyright through 2026 ([LuaJIT](https://github.com/LuaJIT/LuaJIT/blob/v2.1/README)).
- **sol2** (MIT) "supports all Lua versions 5.1+" ([sol2](https://github.com/ThePhD/sol2)).
- **Luau** (MIT) is "gradually typed" and Lua 5.1-compatible ([Luau](https://github.com/luau-lang/luau)). Its documented sandbox ([luau.org](https://luau.org/sandbox)):
  - removes `io` and `package`
  - makes library tables readonly
  - offers an interrupt callback as a CPU limit
  - recommends "separate VMs for trusted and untrusted code"
- **AngelScript** is at 2.38.0 (zlib) ([AngelCode](https://www.angelcode.com/angelscript/)).
- **Wren's** newest changelog entry is 0.4.0, with copyright through 2021 ([Wren](https://github.com/wren-lang/wren/blob/main/CHANGELOG.md)).
- **.NET hosting** APIs "only support framework-dependent deployments" ([.NET docs](https://github.com/dotnet/docs/blob/main/docs/core/tutorials/netcore-hosting.md)). .NET 10 is the LTS release, with end of life on 2028-11-14 ([dotnet/core](https://github.com/dotnet/core/blob/main/release-notes/releases-index.json)).
- **Mono's** upstream moved to WineHQ ([mono](https://github.com/mono/mono)).
- **Wasmtime's** interruption modes ([Wasmtime](https://github.com/bytecodealliance/wasmtime/blob/main/docs/examples-interrupting-wasm.md)):
  - **Fuel** is "completely deterministic."
  - **Epochs** cost "around a 10% slowdown" and are non-deterministic.

**Native code reload.**

- **Live++** patches running machine code. It costs **€119/yr** (Individual, Windows) or **€199/yr** (Business). Console licenses are business-only ([Live++ pricing](https://liveplusplus.tech/pricing.html)).
- **cr.h** (MIT) reloads a module on Linux, macOS and Windows, with crash rollback ([cr](https://github.com/fungos/cr)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Gameplay in C++. Tunables in data files and CVars, reloaded on change. Optional native reload: a DLL-swap module with host-owned state, or a bought patcher on Windows | A tuning change appears in the running game without a rebuild | Programmers spend more time relaunching than editing |
| S2 | Native reload as a development-only feature. Optionally one embedded language (Lua 5.5 with sol2, or Luau) for level, encounter and dialogue logic. Bindings generated from reflection. Scripts hold handles, never pointers | A script error reports file and line without crashing the host. The logic for one shipped level lives in script | Non-programmers need to author logic, or logic repeats across many levels |
| S3 | Interpreter or AOT paths on consoles (console JIT policies are NDA-gated). Confirm .NET runtime availability per platform before choosing it | Scripts run unchanged on every target | Designers outnumber engine programmers |
| S4 | Typed scripting (Luau types, a stable versioned script API shared across titles). Visual or graph tools for designers. Instruction or time budgets, memory caps, a debugger hook, profiler zones per script | A runaway script stops within budget. Scripts appear in the profiler | Live-ops hot-fixes, UGC, or server-run untrusted logic |
| S5 | Separate VMs for trusted and untrusted code. Signed bytecode. Deterministic budgets (fuel) wherever scripts run inside replays or lockstep | Mod or UGC code cannot reach file I/O or host memory. Server hot-fix scripts deploy without a binary patch | — |

Guidance Inference the report's synthesis

**Build vs buy.** Buy the VM and the binding library. Build the binding generator on top of reflection.

**Lock early.** The scripting language and its binding API. Content accumulates in the language, and semantics differ between Lua 5.1/Luau and 5.5. Defer adoption until a trigger fires.

**Pitfalls.**

- Script state in VM globals instead of components, which breaks saves.
- Wall-clock interrupts inside deterministic simulations.
- Accepting bytecode from untrusted sources.
- Designing the engine around DLL-swap constraints too early.

**Learn.** The embedding API of the chosen VM, sandboxing, binding generation, and the Handmade Hero live-reload days.

Why it matters

Content accumulates in the chosen language, so the choice is locked at adoption and should wait for a trigger.

[Back to the category map](#category-map)

### World

#### Scene and world management

Hierarchy, prefabs, then streaming.

What it is

Scene and world management covers the object hierarchy, prefabs (reusable object templates), spatial queries, level loading and streaming.

Key findings Established sourced facts, as observed in early November 2026

**Transforms and dirty flags.** Nystrom's dirty-flag pattern recomputes world transforms once before rendering. In his example, moving four objects eagerly cost ten calculations. He warns that the flag must be set on every change ([Dirty Flag](https://gameprogrammingpatterns.com/dirty-flag.html)).

**Hierarchies in ECS libraries.**

- EnTT offers no built-in hierarchy and suggests a `relationship` component ([EnTT](https://github.com/skypjack/entt/blob/HEAD/docs/md/entity.md)).
- flecs has native `ChildOf` hierarchies with breadth-first `cascade` traversal. Its children are deleted before their parents ([flecs](https://github.com/SanderMertens/flecs/blob/master/docs/HierarchiesManual.md)).

**Prefabs.**

- flecs prefabs share components across instances, allow per-instance overrides and support variants through inheritance ([flecs Prefabs](https://github.com/SanderMertens/flecs/blob/master/docs/PrefabsManual.md)).
- Godot instances packed `.tscn` scenes "into any number of scenes" ([Godot](https://docs.godotengine.org/en/stable/getting_started/step_by_step/instancing.html)).

**Spatial partitioning.** Nystrom's survey ([Spatial Partition](https://gameprogrammingpatterns.com/spatial-partition.html)):

- Flat grids are simple and fast to update, but clustered objects overload cells.
- Object-adaptive trees "work best built once from a full set of objects," so they suit static geometry.

**Large worlds (Unreal World Partition, UE 5.8).** The docs describe it as "distance-based level streaming" ([UE 5.8](https://dev.epicgames.com/documentation/en-us/unreal-engine/world-partition-in-unreal-engine)):

- grid cells (one example uses 256 m cells with a 768 m loading radius)
- streaming sources
- data layers
- One File Per Actor, so editing doesn't require checking out the level file

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | One world. A level file (JSON through reflection). Whole-level load and unload. A parent/child relationship with local and world transforms, propagated parent-before-child once per frame via dirty flags. Children destroyed before parents. Brute-force queries | Loading, unloading and reloading a level 100× leaks nothing and gives identical entity counts. Moving a root updates each descendant exactly once per frame | Query costs show in profiles, or authored object groups need reuse |
| S2 | Prefabs with per-instance overrides, stored as a prefab reference plus overrides even if the runtime instantiates by copy. A spatial index: a loose grid for dynamic objects, a BVH for static geometry, kept current by ECS signals, with radius/AABB/ray/frustum queries. Async level loading. Seeded procedural placement hooks, with the seed stored in the save | Editing a prefab updates every unmodified instance on reload, and overrides survive. AI, audio and gameplay queries beat brute force at the target entity counts. A level transition never exceeds the frame budget outside a loading screen | Levels exceed memory, or seamless transitions are wanted |
| S3 | Per-platform cooked binary scenes. Memory budgets per level or cell. Sub-level streaming | The weakest target holds every level within budget | Merge conflicts on level files, or several designers per map |
| S4 | One file per entity or prefab. Editor loading regions. Nested prefabs and variants shared across titles | Two designers edit one region the same day without conflicts | Seamless open worlds or MMO zones |
| S5 | Cell streaming by distance from streaming sources (players, cinematics, server interest). Priorities. Loaded vs Activated states. HLOD for far cells. Navmesh tiles streamed with cells. Data layers for live events. Large-world coordinates | Traversal stays under the memory budget with no hitches. Streaming sources work in headless server builds | — |

Guidance Inference the report's synthesis

**Build vs buy.** Build it. The patterns are well documented, and the representation is entangled with your ECS.

**Lock early.**

- **Hierarchy and transform ownership.** Fix parent-before-child propagation and children-first destruction.
- **Prefab model.** Store reference plus overrides.
- **Level-file granularity.** Switch to per-entity files once a second designer edits a map.

**Pitfalls.**

- Recomputing transforms eagerly.
- Exposing the spatial structure instead of a query API.
- Monolithic level files on a team.
- Streaming sources that exist only on clients.

**Learn.** Nystrom's Dirty Flag and Spatial Partition chapters, the flecs hierarchy and prefab manuals, and the World Partition docs.

Why it matters

Transform ownership, the prefab model and level-file granularity are set here and affect every level made afterward.

[Back to the category map](#category-map)

#### Cameras

Presentation-only, judged by their failures.

What it is

The camera system chooses the player's view: the base pose, modifiers such as shake, blends and scripted cameras.

Key findings Established sourced facts, as observed in early November 2026

**Design principles.**

- John Nesky (*Journey*'s camera designer) says "Cameras are most noticeable when they fail." His failure classes include undermining direction, breaking line of sight and causing motion sickness ([Game Developer](https://gamedeveloper.com/design/video-50-common-game-camera-mistakes----and-how-to-fix-them)).
- Squirrel Eiserloh's GDC 2016 talk covers framing, smoothing, camera shake and split-screen ([GDC Vault](https://gdcvault.com/play/1023146/Math-for-Game-Programmers-Juicing)).

**Unity's Cinemachine as a reference architecture.**

- **Brain.** It "chooses the active Cinemachine Camera component with the highest Priority," with ties going to the most recently activated ([Cinemachine](https://docs.unity3d.com/Packages/com.unity.cinemachine@3.1/manual/concept-camera-control-transitions.html)).
- **Blends.** "A Cinemachine blend is not a fade, wipe, or dissolve."
- **Timeline.** It overrides priority for cinematics.
- **Deoccluder.** It preserves line of sight with raycasts, a minimum occlusion time and a small camera radius ([Deoccluder](https://docs.unity3d.com/Packages/com.unity.cinemachine@3.1/manual/CinemachineDeoccluder.html)).
- **Impulse.** It propagates shake outward from a point in space to listeners ([Impulse](https://docs.unity3d.com/Packages/com.unity.cinemachine@3.1/manual/CinemachineImpulse.html)).

**PhysX update order.** PhysX recommends updating "moving obstacles first, then the controller, then the camera" ([PhysX CCT](https://nvidia-omniverse.github.io/PhysX/physx/5.6.1/docs/CharacterControllers.html)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | A first-person view from the player pose with look input, FOV and near/far, read from the interpolated render state. A free-fly debug camera | No jitter at 60 fps render with a 60 Hz simulation. FOV and sensitivity are settings | Feel work starts, or a non-gameplay camera is needed |
| S2 | A camera stack: base pose plus clamped additive modifiers (shake, head-bob, FOV kick, recoil). Event-driven shake with source and falloff. A priority-based camera brain with blends and cuts. Scripted cameras that override priority. Third-person collision only if needed (sphere cast, pull-forward, minimum occlusion time, damping) | Gameplay events trigger shake; there are no hard-coded shake calls. Every modifier can be turned off. Cutscene, death and gameplay cameras hand off without pops. No camera ends up inside geometry in a test sweep | More cinematics, or third-person or vehicle modes |
| S3 | Per-platform input curves and dead zones. Safe-area-aware framing. Comfort options (shake off, FOV range) | Comfort options work on every platform | Several designers author cameras |
| S4 | A sequencer for keyframed camera tracks synced to animation and audio. Rig presets as reusable assets | Designers author a cinematic without code | Spectating, replays or esports |
| S5 | Spectator, replay and kill-cam cameras driven by replicated or recorded state. Cameras stay client-local and never act as authority | Spectators see no authority-only data | — |

Guidance Inference the report's synthesis

**Build vs buy.** Build it. No standalone C++ camera middleware turned up, so Cinemachine serves as a reference design.

**Lock early.** Little. Keep a rig-and-modifier interface, and keep cameras out of the simulation tick.

**Pitfalls.**

- Cameras updated before the character they follow.
- Shake with no off switch, an accessibility failure.
- Flicker from brief occlusions.
- Replicating cameras.

**Learn.** Nesky's failure classes, Eiserloh on smoothing and shake, and the Cinemachine concept docs.

Why it matters

Keeping cameras in presentation, outside the simulation tick, protects determinism and headless servers.

[Back to the category map](#category-map)

#### AI, perception and navigation

Shipped AI is simpler than its reputation.

What it is

Gameplay AI covers how agents decide (state machines, behavior trees), what they perceive, and how they find paths on a navigation mesh.

Key findings Established sourced facts, as observed in early November 2026

**Decision architectures that shipped.**

| Game | Architecture | Notes |
| --- | --- | --- |
| Left 4 Dead | Hierarchical FSM with a stack | Each transition returns a result that "carries a reason string for debug output" ([Booth 2009](https://steamcdn-a.akamaihd.net/apps/valve/2009/ai_systems_of_l4d_mike_booth.pdf)) |
| Halo 2 | Behavior DAG | \~115 behaviors, \~10,350 tuning numbers tamed by `.character` inheritance files, and an AI target of "30Hz or more" ([Isla 2005](https://gamedeveloper.com/programming/gdc-2005-proceeding-handling-complexity-in-the-i-halo-2-i-ai)) |
| F.E.A.R. | GOAP | "Three states" plus an A\* planner over a fixed-size world-state array ([Orkin 2006](https://alumni.media.mit.edu/~jorkin/gdc2006_orkin_jeff_fear.pdf)) |

**Behavior trees.**

- Many games tick trees "every other frame or at about 5 Hz" ([Champandard & Dunstan](http://www.gameaipro.com/GameAIPro/GameAIPro_Chapter06_The_Behavior_Tree_Starter_Kit.pdf)).
- BehaviorTree.CPP (MIT) is at 4.10.0 and loads XML trees at runtime. Its Groot2 editor's free tier is capped at 20 nodes; PRO is €590/yr ([BT.CPP](https://github.com/BehaviorTree/BehaviorTree.CPP/blob/HEAD/CHANGELOG.rst); [Groot2](https://www.behaviortree.dev/groot)).

**Utility AI.** Utility scoring "can be extremely fast … and it scales very well", but curve design "is really more art than science" ([Graham](http://www.gameaipro.com/GameAIPro/GameAIPro_Chapter09_An_Introduction_to_Utility_Theory.pdf)).

**Directors.**

- L4D's director "adjusts frequency (pacing), not amplitude (difficulty)." It runs a Build Up → Sustain Peak (3–5 s) → Peak Fade → Relax (30–45 s) cycle ([Booth 2009](https://steamcdn-a.akamaihd.net/apps/valve/2009/ai_systems_of_l4d_mike_booth.pdf)).
- Alien: Isolation's director "always knows the player's location," while the alien "has to figure it out for itself." This comes from a secondary source ([Game Developer](https://www.gamedeveloper.com/design/the-perfect-organism-the-ai-of-alien-isolation)).

**Perception.** Unreal's perception system gives each sense its own configuration, with sight radius, lose-sight radius, peripheral half-angle, hearing range and a per-sense Max Age ([UE 5.8 AI Perception](https://dev.epicgames.com/documentation/en-us/unreal-engine/ai-perception-in-unreal-engine)).

**Navigation (Recast/Detour).**

- zlib-licensed; the latest release is 1.6.0 from 2023. It "powers AI navigation features in Unity, Unreal, Godot, O3DE" ([Recast](https://github.com/recastnavigation/recastnavigation)).
- Only tiled meshes support re-baking, hierarchical planning and streaming.
- Agent radius, height, slope and step are baked into the mesh.
- It "provides no specific threading support" and has no stable C API ([Recast docs](https://github.com/recastnavigation/recastnavigation/blob/HEAD/Docs/_99_Roadmap.md)).

**Debugging.** Unreal's Gameplay Debugger works "even on clients in networked games using replication" and is compiled out of shipping builds ([UE Gameplay Debugger](https://dev.epicgames.com/documentation/en-us/unreal-engine/using-the-gameplay-debugger-in-unreal-engine)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | A per-agent FSM in C++ that logs a reason string for every transition. On-screen state labels. Perception: sight (radius, lose-sight radius, half-angle, line-of-sight ray), hearing from gameplay noise events, stimuli that age and expire, last known position. Fixed waypoints or A\* on a hand-placed graph | Every transition shows in an overlay with its reason. AI reacts only to perceived data and is deterministic under replay with seeded RNG | Designers need agents to reach arbitrary points, or hand-placed graphs break with level edits |
| S2 | An HFSM with a stack, an event-driven BT, or utility scoring with inertia. Static tree data shared, instance data per agent. A Recast bake in the content cook. Detour queries with reactive look-ahead path following. Off-mesh links. A director with an intensity metric, if the genre needs pacing. An in-engine AI debugger | A new enemy variant reuses most behaviors through data. No oscillation between behaviors in soak tests. Agents reach any reachable point in test levels. A fairness audit passes: no unseen teleports, no spawns in view | Behavior count outgrows code-only FSMs, or tuning parameters explode |
| S3 | AI budgets: sliced pathfinding with per-frame iteration caps, BT ticks at 5–10 Hz with event wake-ups, AI LOD for weak CPUs, navmesh cooked per platform | AI CPU stays within budget on the weakest target | Designers are blocked on programmers |
| S4 | Data-driven trees and characters with inheritance. Visual editors. Recorded AI timelines (state, BT node, perception) with replay. Utility or GOAP for systemic games | A designer diagnoses "why did it do that?" from a recording | Thousands of agents, or sharded worlds |
| S5 | Server-only AI with tick-rate LOD. Tiled navmesh streaming with world cells. TileCache obstacles. Crowd avoidance. Replicated debug categories | AI CPU holds at target agent counts. Navmesh tiles stream with cells | — |

Guidance Inference the report's synthesis

**Build vs buy.**

- **Navigation:** buy Recast/Detour; nearly every engine does.
- **Decision-making:** build the layer, or use BT.CPP with the free Groot2 tier.
- **Perception and the director:** build them; they are game-specific.

**Lock early.**

- **The director/agent knowledge boundary.** An agent's blackboard has no player-position field; the director only writes hints.
- **Navmesh tiling and agent classes.** Start tiled, even with one tile.
- **Versioned AI data formats.**

**Pitfalls.**

- Agents reading global state.
- Unordered hash maps in blackboards.
- Wall-clock stimulus ages; use ticks instead.
- Using the navmesh as collision geometry, which Recast warns against ([Recast Introduction](https://github.com/recastnavigation/recastnavigation/blob/HEAD/Docs/_1_Introduction.md)).
- Recast calls from multiple threads.

**Learn.**

- Red Blob's BFS → Dijkstra → A\* progression ([Red Blob Games](https://www.redblobgames.com/pathfinding/a-star/introduction.html)).
- The Booth, Isla and Orkin papers.
- The BT Starter Kit.
- Utility curves.
- The Recast documentation.

Why it matters

The boundary on what an agent may know, and the navigation-mesh tiling, are early decisions that later tooling depends on.

[Back to the category map](#category-map)

#### Procedural generation

Seeds are save data.

What it is

Procedural generation builds levels or content from a seed using rules and templates.

Key findings Established sourced facts, as observed in early November 2026

**Spelunky's guaranteed path.** Spelunky's generator builds "16 rooms in a 4x4 grid" ([Kazemi](https://tinysubversions.com/spelunkyGen/)):

- It first walks a solution path that forces room types to guarantee connectivity.
- It then adds "some random type 0 rooms" off that path.

**Wave Function Collapse.** WFC (MIT) produces output "locally similar to the input." It can hit "a contradiction" during propagation, although these are "surprisingly rare" in practice ([WaveFunctionCollapse](https://github.com/mxgmn/WaveFunctionCollapse)).

**Portability of standard distributions.** P2059R0 states that their output "is implementation-specified and thus can (and does) differ across implementations" ([P2059R0](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p2059r0.pdf)).

**Baking.** Lightmaps need unique UVs per mesh ([xatlas](https://github.com/jpcy/xatlas)). DDGI can run without baking, but only on ray-tracing backends ([RTXGI-DDGI](https://github.com/NVIDIAGameWorks/RTXGI-DDGI)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | An engine-owned PRNG and range mapping (see [Random numbers](#rng)). Template rooms assembled on a grid around a guaranteed critical path | The same seed gives a byte-identical layout hash | Bugs that can't be reproduced, or unfair layouts |
| S2 | Post-generation validators (reachability, objective placement, spawn distances) that reject and re-roll with a derived sub-seed. Independent streams per subsystem (layout, loot, AI, cosmetic). Seed shown in UI and logs. A "load seed N" command. Overlays of template IDs and constraint failures. A batch generator that renders thumbnails | Validator pass rate and re-roll counts are measured over 10k seeds. Changing cosmetic code leaves layout hashes unchanged. Any bug report with a seed reproduces in one step | A second compiler or platform |
| S3 | Cross-platform layout-hash tests as a release gate | Hashes match on every shipping toolchain | Designers want to add content |
| S4 | A template editor (rooms as data, tags, weights). WFC or grammar fill for decoration. Designer-tunable rules | Designers add templates without code | Online or streamed worlds |
| S5 | Server-authoritative generation. A shared seed with deterministic client generation for prediction. Chunked or streamed generation | Client and server agree on generated content (hash check on join) | — |

Guidance Inference the report's synthesis

**Build vs buy.** Build the generator and its tools. WFC (MIT) and its C++ ports are an optional fill technique.

**Lock early.** Determinism ([#9](#contract-9), [#10](#contract-10)). Also the cross-cutting rule that runtime-assembled layouts rule out whole-level lightmaps, baked PVS and level-wide probe placement. Bake per template instead, or rely on dynamic lighting with shadow caching.

**Pitfalls.**

- RNG draws in unordered-container order.
- Generators that loop forever on contradictions.
- Constraints checked only by playtesting.
- Floats in generation logic. Integer-only logic is the safer default.

**Learn.** PRNG stream splitting, BFS/DFS reachability, constraint propagation, and basic statistics for tuning distributions.

Why it matters

Seeds are save data, and levels assembled at runtime rule out several baked rendering techniques.

[Back to the category map](#category-map)

## CategoryInterface

Input is the only player-facing system that enters the simulation; UI, text, localization and accessibility presentation all stay in the presentation layer.

Overview

Input is the only player-facing system that enters the simulation. Raw events are folded into one command per tick, and that command is also the unit of replay and network transmission ([Gambetta](https://www.gabrielgambetta.com/client-side-prediction-server-reconciliation.html)).

UI, text, localization, accessibility presentation and video all run at render rate in the presentation layer. They read simulation state through a binding layer and persist through a versioned settings file kept separate from saves.

The shipping floor for a PC game with controller and handheld support comes from published rules:

- **Steam Deck Verified:**
  - glyphs that match the active device
  - an on-screen keyboard for text entry
  - a default configuration that reaches all content
  - text no smaller than 9 px at 1280×800 (12 px recommended) ([Steam Deck compatibility](https://partner.steamgames.com/doc/steamdeck/compat))
- **Accessibility guidelines:** the Game Accessibility Guidelines' Basic tier ([GAG](https://gameaccessibilityguidelines.com/full-list/)).

### Interaction

#### Input

Gameplay reads actions, never devices.

What it is

The input system turns device events into named actions that gameplay reads, and folds them into one command per simulation tick.

Key findings Established sourced facts, as observed in early November 2026

**SDL3 devices and text.**

- "a gamepad tells you *where* a button or axis is on the device." Mappings are configuration strings, and "Your application should always support gamepad hotplugging." In a Steam game, call `SteamAPI_InitEx()` before `SDL_Init()` ([SDL3 CategoryGamepad](https://wiki.libsdl.org/SDL3/CategoryGamepad)).
- The community controller DB is zlib-licensed ([SDL_GameControllerDB](https://github.com/mdqinc/SDL_GameControllerDB)).
- Scancodes "are guaranteed to reference the physical location on the keyboard." For text entry, "you should not be using key press events!" ([SDL3 BestKeyboardPractices](https://wiki.libsdl.org/SDL3/BestKeyboardPractices)).
- Starting text input can raise an on-screen keyboard or IME that "can prevent some key press events from being passed through" ([SDL_StartTextInput](https://wiki.libsdl.org/SDL3/SDL_StartTextInput)).
- Every event carries a nanosecond timestamp ([SDL_CommonEvent](https://wiki.libsdl.org/SDL3/SDL_CommonEvent)).

**Reference action system: Unreal Enhanced Input** ([Unreal Enhanced Input](https://dev.epicgames.com/documentation/en-us/unreal-engine/enhanced-input-in-unreal-engine)).

- Actions are "the communication link between the Enhanced Input system and your project's code."
- Mapping contexts are prioritized and stackable.
- It provides modifiers (dead zones), triggers (hold, tap, chord) and player-mappable presets.
- `InjectInputForAction` simulates input for tests.

**Steam Input** ([Steam Input](https://partner.steamgames.com/doc/features/steam_controller/getting_started_for_devs)).

- Steam re-presents gamepads so they "appear in your game as regular Xbox controller input."
- Its native API caps actions at "16 analog and 128 digital actions."
- Valve recommends you "re-gather the origins each frame" for glyphs.

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Platform events polled into per-frame device state. A thin action table, even if hard-coded. Scancodes for movement. Gamepads through the standard abstraction plus the controller DB. Hotplug handled | Playable on keyboard/mouse and one gamepad. Unplug/replug mid-game works. No gameplay code names a physical key | First external playtest, or a settings screen |
| S2 | Data-driven actions with prioritized, stackable contexts (gameplay, menu, modal, text entry). Dead-zone and sensitivity modifiers. Hold/tap/toggle triggers, plus a hold→toggle option. A rebinding UI with conflict detection and per-device presets. Bindings persisted as scancode + device class. Last-used-device glyph switching. Steam Input (legacy or native). Text input and IME only while a field has focus; the platform on-screen keyboard on handhelds. Events folded into one command per sim tick by timestamp | Deck Verified input items pass. Every action is remappable and survives a restart. A tap shorter than one tick is never dropped. Recorded command streams replay identically | A console port, a second input backend, or local multiplayer |
| S3 | Per-platform device backends behind one action API. Per-user device pairing. Platform glyph sets and text-entry dialogs. Suspend/resume handling | The same action assets drive every platform | More designers author input; several titles |
| S4 | Actions, contexts, triggers and modifiers as versioned designer assets. An input debug overlay. Input injection and replay in CI. A shared glyph library | A new title adds an action with no engine code. CI runs replayed-input tests | Online play with untrusted clients |
| S5 | Commands with sequence numbers, validated and rate-limited on the server. Client prediction and reconciliation. Chat input with IME across all locales | The server rejects malformed or impossible command streams | — |

Guidance Inference the report's synthesis

**Build vs buy.** Buy the device layer and the controller database. Build the action layer.

**Lock early.** Contract [#13](#contract-13). Retrofitting rebinding, contexts, Steam action sets or hold→toggle requires gameplay that already reads actions.

**Pitfalls.**

- Text entry built from key events.
- Text input left active during gameplay.
- Choosing glyphs by USB ID while Steam presents everything as Xbox.
- Showing keycode names for scancode bindings without converting through the current layout.

**Learn.** OS event loops, scancodes vs keycodes vs text events, IME composition, dead-zone math, Steam Input action sets.

Why it matters

Action-based input (contract [#13](#contract-13)) is the precondition for rebinding, accessibility options, Steam Input and replays.

[Back to the category map](#category-map)

#### UI

Commit to one shipping framework before the first settings screen.

What it is

The UI system draws menus and the HUD and handles focus navigation with a controller or mouse.

Key findings Established sourced facts, as observed in early November 2026

**Debug UI is not game UI.** Dear ImGui's FAQ: "Dear ImGui is NOT designed to create a user interface for games" ([Dear ImGui FAQ](https://github.com/ocornut/imgui/blob/master/docs/FAQ.md)).

**Free options.**

- **RmlUi** (MIT, C++17) is "based on the HTML and CSS standards." It offers data binding and "Spatial navigation" ("Suitable for controllers"), but doesn't "aim to be fully compliant with CSS or HTML." Version 6.3 added IME support for SDL backends ([RmlUi](https://github.com/mikke89/RmlUi); [RmlUi releases](https://github.com/mikke89/RmlUi/releases)).
- **Clay** (zlib) is a "Single 4.8k LOC clay.h file" with "Microsecond layout performance." It needs a fast app-supplied text-measure callback and "does not currently support proper multi-threading" ([Clay](https://github.com/nicbarker/clay)).
- **Yoga** (MIT) is an embeddable flexbox engine ([Yoga](https://github.com/facebook/yoga)).

**Paid middleware.**

| Product | Tiers | Console support |
| --- | --- | --- |
| NoesisGUI (XAML, perpetual, "per project, per platform") | Indie: **€195** per project for budgets under €250K. Pro: **€9,000** for the first platform plus €3,600 per extra platform ([Noesis](https://www.noesisengine.com/licensing.php)) | Xbox, PlayStation and Switch listed on every tier |
| Ultralight | Free under $200K turnover (60 FPS cap). Indie: $395. Pro: $4,500 ([Ultralight](https://ultralig.ht/pricing/)) | Enterprise tier only |

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Dear ImGui for tools only. A minimal custom menu/HUD on the engine's 2D batcher, optionally laid out with Clay or Yoga | Main menu → play → pause → quit works on keyboard and gamepad | A real settings or remap screen, localization, or subtitles |
| S2 | One shipping approach: custom UI over Clay/Yoga and the engine text service, RmlUi, or Noesis/Ultralight indie tiers. Required: gamepad focus and visible focus, resolution-independent scaling with 9/12 px floors at 1280×800, glyph prompts, localized strings, a subtitle renderer, world-space UI via render targets, atlased batching | Every screen works gamepad-only and mouse-only. Text never falls below 9 px at 1280×800. Pseudo-localization (+40%) shows no clipping | A console port, a second title, or non-programmer UI authors |
| S3 | Per-platform safe areas, platform-mandated screens and glyphs. The framework must build for consoles | UI runs unchanged on every target except platform screens | Team growth |
| S4 | Designer-authored markup with hot reload. A data-binding layer between ECS state and view models. UI automation tests. An accessibility-tree export for narration | UI artists ship a screen with no engine changes. A screen reader can read the menus | Live-ops scale |
| S5 | Virtualized lists (inventory, chat, social). Server-driven UI content | Large lists stay within the frame budget | — |

Guidance Inference the report's synthesis

**Build vs buy.** Either works. Price the console tier before choosing middleware.

**Lock early.** The shipping framework and authoring language. Switching later means re-authoring every screen.

**Pitfalls.**

- Shipping debug ImGui as game UI.
- Assuming RmlUi accepts web HTML/CSS as-is.
- Clay's measure callback on the hot path.
- Middleware whose console tier you can't afford.
- No focus model, which leaves controller navigation as an afterthought.

**Learn.** Retained vs immediate-mode trade-offs, flexbox, 2D batching and atlases, focus/navigation graphs, MVVM data binding.

Why it matters

The shipping framework and its authoring language are a lock-in: switching later means re-authoring every screen.

[Back to the category map](#category-map)

### Language

#### Text and fonts

Shape runs, not characters.

What it is

Text and fonts cover rasterizing glyphs, shaping (turning characters into positioned glyphs for a script), fallback fonts and layout.

Key findings Established sourced facts, as observed in early November 2026

**Font rasterizing and shaping.**

- FreeType offers two licenses. The FTL is "a BSD-style license with a credit clause," compatible with GPLv3 but not GPLv2 ([FreeType license](https://freetype.org/license.html)).
- HarfBuzz is used in Chrome, Firefox, Android, PlayStation, Godot and Unreal. It doesn't hint glyphs, and its maintainers say "We will never break the ABI" ([HarfBuzz](https://github.com/harfbuzz/harfbuzz)). It is listed as MIT-licensed ([SDL_ttf](https://github.com/libsdl-org/SDL_ttf)).
- SDL3_ttf (zlib) wraps FreeType and HarfBuzz with SDF rendering, fallback fonts, script and direction settings, and a GPU text engine ([SDL3_ttf](https://wiki.libsdl.org/SDL3_ttf/CategorySDLTTF)).

**Distance-field atlases.** msdf-atlas-gen (MIT) builds SDF, MSDF and MTSDF atlases and adds glyphs on demand. Its immediate generator "may launch new threads every time" ([msdf-atlas-gen](https://github.com/Chlumsky/msdf-atlas-gen)).

**Bidirectional text.**

- SheenBidi (Apache-2.0) implements the Unicode Bidirectional Algorithm ([SheenBidi](https://github.com/mta452/SheenBidi)).
- FriBidi is LGPL-2.1+ ([FriBidi](https://github.com/fribidi/fribidi)).

**Glyph ranges.** Dear ImGui 1.92 stopped requiring declared glyph ranges ([Dear ImGui FAQ](https://github.com/ocornut/imgui/blob/master/docs/FAQ.md)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | A pre-baked bitmap atlas or a simple TrueType rasterizer. Latin text. UTF-8 bytes end to end | Strings are UTF-8 everywhere | The first non-Latin language, or scalable world-space text |
| S2 | FreeType + HarfBuzz shaping, directly or through SDL3_ttf. MSDF/MTSDF atlases for scalable HUD and diegetic text. A dynamic glyph atlas with a CJK fallback chain. Line breaking on spaces plus any-character breaks for CJK, with manual breaks for subtitles. A layout API that takes runs, not characters | Shipped languages render without tofu. Latin kerning and ligatures are correct. CJK fits the atlas memory budget | An RTL or complex-script language, or consoles |
| S3 | Bidi plus script itemization feeding HarfBuzz. Mirrored UI. Per-platform font licensing | Arabic and Hebrew render and wrap correctly in mirrored UI | A text service shared across titles |
| S4 | One text-layout service for UI middleware and in-world text. Font subsetting in the cook. Glyph-cache budgets | Middleware and engine text produce identical metrics | User-generated text, emoji |
| S5 | Arbitrary user text in any script, with a full fallback stack and color fonts | Any player name renders without tofu | — |

Guidance Inference the report's synthesis

**Build vs buy.** Buy rasterizing and shaping. Build the run-based layout API and the atlas policy.

**Lock early.** Contract [#14](#contract-14). Bidi can be added at S3 without breaking callers, but only if layout already works on runs.

**Pitfalls.**

- Per-character APIs that can't express shaping.
- Fixed glyph ranges.
- Atlas generators that spawn threads.
- Forgetting FreeType's credit clause.
- Mixing GPLv2 code with FTL.

**Learn.** Code points vs grapheme clusters vs glyphs, OpenType shaping, UAX #9, distance-field rendering, atlas packing.

Why it matters

A layout API built on runs instead of characters (contract [#14](#contract-14)) is what allows complex scripts to be added later.

[Back to the category map](#category-map)

#### Localization

Keys and whole messages from the first string.

What it is

Localization is the system for translating strings and assets: string keys, plural rules, message formatting and pseudo-localization.

Key findings Established sourced facts, as observed in early November 2026

**Steam's language rules.** Steam lists 30 fully supported languages, "the only set of languages supported by Steamworks APIs," and "English is Steam's fallback language and needs to have content" ([Steam Languages](https://partner.steamgames.com/doc/store/localization/languages)).

**Plural rules (CLDR).** The categories are zero, one, two, few, many and other. Languages use different subsets ([CLDR](https://www.unicode.org/cldr/charts/latest/supplemental/language_plural_rules.html)):

| Language | Cardinal plural categories |
| --- | --- |
| Arabic | 6 |
| French | 3 |
| English | 2 |
| Japanese, Chinese | 1 |

**ICU message formatting.**

- Translate whole messages. With fragments, "translators would not be able to rearrange the pieces."
- Put `select` outermost and nest `plural` inside it ([ICU](https://unicode-org.github.io/icu/userguide/format_parse/messages/)).
- ICU 79 shipped on 2026-10-22. Some of its MessageFormat 2 API "remains in technology preview" ([ICU downloads](https://unicode-org.github.io/icu/download)).

**Fluent.** Fluent lets each translation set its own complexity, but it lists no C++ implementation ([Project Fluent](https://projectfluent.org/)).

**Pseudo-localization (Microsoft).**

- Pad text by about 40%. Short strings can grow "200% or even 400%."
- Use mirrored and East Asian pseudo-locales ([Microsoft](https://learn.microsoft.com/en-us/globalization/methodology/pseudolocalization)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Every player-visible string goes through `loc(key)` into one table, even with one language. No concatenation | A grep finds no literal UI strings in code | A second language, or an external translator |
| S2 | Each entry holds key, source text, context comment and max length. Plural and select via CLDR categories (ICU MessageFormat v1, or a Fluent-like format behind `format(key, args)`). A pseudo-locale build. Default language from the store, with English fallback. Per-language font sets. Localized subtitle and audio-bank paths | Pseudo-loc passes clean. Plural tests cover English (2), French (3) and Arabic (6), even if Arabic doesn't ship | More languages, VO, consoles |
| S3 | Localized VO and asset bundles. Platform terminology tables. Number and date formatting through ICU skeletons. RTL | Each language installs or toggles independently | Translation vendors, several titles |
| S4 | A translation-management pipeline. String freeze. Linguistic QA with context screenshots. Shared terminology | Strings flow from designer to translator to build without engineers | Live ops |
| S5 | Server-delivered, hot-updatable string tables. Messages sent as key + arguments so each client renders its own language | A string hotfix ships without a client patch | — |

Guidance Inference the report's synthesis

**Build vs buy.** Buy ICU or a plural-rules library. Build the table, the pseudo-locale and the pipeline.

**Lock early.** Contract [#14](#contract-14). Saves store keys and arguments, never localized text.

**Pitfalls.**

- Concatenating strings.
- Letting translators edit date patterns.
- Assuming two plural forms.
- Budgeting too little text expansion.
- Forgetting the English fallback.

**Learn.** CLDR plural rules, ICU MessageFormat or Fluent, BCP-47 tags, pseudo-localization.

Why it matters

Keys and whole-message formatting from the first string (contract [#14](#contract-14)) keep translation from forcing code changes.

[Back to the category map](#category-map)

### Access

#### Accessibility

Architectural hooks in S1, the guideline floor in S2.

What it is

Accessibility covers the options and presentation rules that let more players use the game: remapping, subtitles, flash limits and narration.

Key findings Established sourced facts, as observed in early November 2026

**GAG tiers** ([GAG](https://gameaccessibilityguidelines.com/full-list/)).

- **Basic:**
  - remappable controls
  - adjustable game speed
  - haptics toggle
  - UI reachable with the gameplay input method
  - no flicker
  - no color-only information
  - separate volume controls
  - subtitles for important speech
  - saved settings
  - several difficulty levels
  - readable default font size
- **Intermediate:** alternatives to held buttons, and subtitles on before any sound plays.
- **Advanced:** screen reader support, adjustable font size, and "Do not make precise timing essential to gameplay."

**Xbox Accessibility Guidelines.** The XAGs are at V3.2, numbered 101–123. They say they "aren't intended to act as a checklist to validate any type of compliance or legal requirements" ([XAG](https://learn.microsoft.com/en-us/gaming/accessibility/guidelines)).

**Subtitles (XAG 104)** ([XAG 104](https://learn.microsoft.com/en-us/gaming/accessibility/xbox-accessibility-guidelines/104)):

- Lines of 40 characters or fewer.
- No more than two lines.
- Scalable to 200%.
- Adjustable background opacity.
- Speaker names.
- Captions for non-speech sounds, with direction indicators.
- Categories that toggle independently.
- Platform caption settings used by default.

**Photosensitivity (XAG 118).** Content fails at "approximately more than three" flashes per second covering "approximately 20 percent or more" of the screen. Removing such content "is preferred over splash-screen warnings" ([XAG 118](https://learn.microsoft.com/en-us/gaming/accessibility/xbox-accessibility-guidelines/118)).

**Screen readers for custom UI.** AccessKit (Apache-2.0 or MIT) bridges self-drawn UI to Windows UIA, macOS and AT-SPI. It suits immediate-mode GUIs "if they provide stable IDs" ([AccessKit](https://github.com/AccessKit/accesskit)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Hooks only: an action layer ready for remapping, separate audio buses, a settings file, a subtitle data model (speaker, category, direction, manual breaks), no color-only signals | The hooks exist and are used | First external playtest |
| S2 | The full GAG Basic tier. XAG 104 subtitle presentation. Hold→toggle. Sensitivity and haptics sliders. Game speed, difficulty and assist options driven by the simulation's time scale. A flash review against XAG 118 thresholds. Accessibility features documented in-game and on the store page | A GAG Basic audit passes. Subtitles are on, or chosen, before the first audio. No flash sequence exceeds \~3/s over \~20% of the screen | Console ports, or requests for screen-reader support |
| S3 | A full XAG pass. Platform caption defaults. Menu narration through AccessKit or OS text-to-speech. Audio description | Menus work with the system screen reader on PC | Consistency across titles |
| S4 | A shared accessibility component library. Settings profiles. Regular testing with disabled players | Every title ships the same baseline at no extra cost | Online social features |
| S5 | Speech-to-text and text-to-speech chat (XAG 119), accessible communication and reporting (XAG 120), accessible support (XAG 122) | Voice and text chat are bridged both ways | — |

Guidance Inference the report's synthesis

**Build vs buy.** The guidelines are free checklists. Buy AccessKit for narration. Build everything else into the input, UI, audio and simulation layers.

**Lock early.** The subtitle data model and the settings schema. Both are hard to retrofit.

**Pitfalls.**

- Splash-screen seizure warnings in place of fixing the content.
- Subtitle data with no speaker or category fields.
- Settings that aren't persisted.

**Learn.** GAG and XAG, captioning conventions, OS accessibility APIs, luminance and contrast math.

Why it matters

Its data models (subtitles, settings) are hard to retrofit, while the published guidelines give a testable floor for stage 2.

[Back to the category map](#category-map)

## CategoryTools

Each tool should arrive at the stage where its pain becomes measurable; the sources support triggers for tools, not a fixed order.

Overview

The sources support **triggers** for tools, not a fixed order:

- **Team contention.** Unreal's One File Per Actor exists because of it ([Unreal OFPA](https://dev.epicgames.com/documentation/en-us/unreal-engine/one-file-per-actor-in-unreal-engine)).
- **Team-wide cost sharing.** It justifies a shared derived-data cache ([Unreal DDC](https://dev.epicgames.com/documentation/unreal-engine/using-derived-data-cache-in-unreal-engine)).
- **Headcount.** Trunk-based development says teams with "more than a couple of developers" need a build server ([trunkbaseddevelopment.com](https://trunkbaseddevelopment.com/)).
- **Early decisions.** Our Machinery warns that "Making decisions upfront, when you have the least information" is a poor time to decide ([The Truth](https://ruby0x1.github.io/machinery_blog_archive/post/the-story-behind-the-truth-designing-a-data-model/)).

**Five rules (inference) keep tools from becoming productive avoidance:**

1. Every tool task names the game-side symptom it removes and how that symptom is measured.
2. Buy before building. A tool with a credible library or service behind it gets a one-day integration spike, not a custom build.
3. Each tool phase is timeboxed and ends at its "done when" line.
4. No tools for data that doesn't exist yet. That means no level editor before several levels exist, and no golden images before the renderer stabilizes.
5. Tools that protect players or data at release are deadlines, not optimizations: crash capture and symbols, deterministic packs, consent gating, reproducible builds.

### Authoring

#### Editor

An overlay that grows into an editor only when people need it.

What it is

The editor is the tooling for inspecting and editing the game world. It starts as a debug overlay and grows into a level editor.

Key findings Established sourced facts, as observed in early November 2026

**Libraries.**

- Dear ImGui v1.93.0 (MIT, tagged 2026-10-14) has a docking branch that "includes: Docking features \[and\] Multi-viewport features" ([Dear ImGui](https://github.com/ocornut/imgui); [FAQ](https://github.com/ocornut/imgui/blob/master/docs/FAQ.md)).
- ImGuizmo (MIT) manipulates "4x4 float matrices." Its last tag is from 2021, but its master branch is active ([ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo)).
- im3d (MIT) adds immediate-mode 3D gizmos ([im3d](https://github.com/john-chapman/im3d)).

**Data model (Our Machinery's The Truth)** ([The Truth](https://ruby0x1.github.io/machinery_blog_archive/post/the-story-behind-the-truth-designing-a-data-model/)):

- Changes are recorded as (object, property, old value, new value) tuples, which drive undo/redo and real-time collaboration instead of "relying on something like the Command Pattern."
- The lessons it drew from Bitsquid include that "copy/paste, undo, and JSON parsing were repeated across tools."

**Team-scale files (Unreal OFPA)** ([Unreal OFPA](https://dev.epicgames.com/documentation/en-us/unreal-engine/one-file-per-actor-in-unreal-engine)):

- One File Per Actor is editor-only: "All Actors are embedded in their respective Level files when cooked."
- Changelists need validation, because a user can submit only some files, "possibly leaving dangling references."

**Reflection for inspectors.**

- GCC 16.1 lists C++26 reflection behind `-freflection` ([isocpp](https://isocpp.org/blog/2026/04/gcc-16.1)).
- Unreal maps project settings to console variables through reflection metadata ([Unreal console variables](https://dev.epicgames.com/documentation/en-us/unreal-engine/console-variables-cplusplus-in-unreal-engine)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | ImGui (docking) windows: entity list, reflected property inspector, log, CVars, stats. Hand-authored text scenes | Any component field can be inspected and tweaked live with no per-type UI code | Hand-editing scene files costs more than about an hour a week |
| S2 | Only for workflows the game repeats: ray-pick selection, transform gizmos, spawn/delete, save to the text scene format. Property-diff undo (entity GUID, component, field path, old, new). Play-in-editor that snapshots and restores through the save path | A level can be blocked out without hand-editing files. Random edit/undo/redo fuzzing returns a byte-identical world. 100 play-in-editor cycles leave the world hash unchanged and leak nothing | Platform ports; the editor stays PC-only and previews arrive through cooking |
| S3 | No editor change. Platform previews through cooking | — | The first non-programmer, or a second level designer |
| S4 | One file per entity or prefab, stable field ordering, GUID references, pre-submit validation, binary-asset locking. A separate editor process (a game crash doesn't take the editor down), plugins, an asset browser, thumbnails | Two people edit one level the same day with no merge conflict and no dangling references. Non-programmers use the editor daily | A distributed team blocked on live collaboration |
| S5 | A server-authoritative in-memory data model that syncs change tuples. Server-side world-editing tools | Multiple users see each other's edits live | — |

Guidance Inference the report's synthesis

**Build vs buy.** Buy the UI and gizmos. Build the inspector, undo, play-in-editor and scene format; each is a thin layer over reflection and serialization.

**Lock early.** One reflection layer ([#4](#contract-4)) and an ID-referenced, merge-friendly scene format.

**Pitfalls.**

- Editor features the current game doesn't need.
- Command-pattern undo that drifts from the data.
- Play-in-editor that leaks static state.
- Scene files with nondeterministic ordering.
- ImGui ID collisions in generic inspectors.
- Editor code leaking into runtime builds.

**Learn.** Immediate-mode UI, reflection (macros, codegen, C++26), ray-pick math, transactional-diff undo, git merge mechanics for text formats.

Why it matters

It reuses the reflection layer (contract [#4](#contract-4)), and its timing is the clearest case of building a tool only on a measured need.

[Back to the category map](#category-map)

#### Hot reload

Data first, code only when measured.

What it is

Hot reload applies changed data, shaders, assets or code to the running game without a restart.

Key findings Established sourced facts, as observed in early November 2026

**File watching.** efsw 1.7.3 (MIT) watches files through inotify, IOCP, FSEvents and kqueue. Modification events "may be reported multiple times during a copy operation," so readers should wait before opening the file ([efsw](https://github.com/SpartanJ/efsw)).

**Live++ (native code patching)** ([Live++ docs](https://liveplusplus.tech/docs/documentation.html)):

- It "compiles your code changes in the background and directly patches the machine code."
- It requires specific compiler and linker flags.
- "Object files built using LTCG/LTO … cannot be used by Live++."
- "Objects created on the stack cannot be migrated to a new class layout."
- For data-layout changes, it recommends serialize/recreate hooks or "reloading the current level in a post-patch hook."

**cr.h (DLL-swap reload).** cr.h works on Linux, macOS and Windows, with "automatic crash protection." Its safe mode warns against holding pointers to statics ([cr](https://github.com/fungos/cr)).

**Config precedence.** Unreal applies console variables by priority, so "all settings can be applied in any order" ([Unreal console variables](https://dev.epicgames.com/documentation/en-us/unreal-engine/console-variables-cplusplus-in-unreal-engine)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | A watcher thread → a debounced event queue → main-thread apply at a frame boundary. Config/CVar reload through the precedence system. Shader reload with a last-good fallback. Asset reload behind handles | A tuning edit lands within a second. 100 reloads of a texture, mesh or sound leak nothing and leave no dangling handles. A broken shader keeps the last good version | The edit-compile-restart loop is measured as a top time sink |
| S2 | Optional code reload: a bought patcher on Windows, or DLL-swap with host-owned state and versioned state structs. Layout changes trigger a level reload | Editing a gameplay function applies without losing play state | Console devkits |
| S3 | Remote file serving to devkits and other machines | Content and code edits reach a devkit without a full deploy | Artists on many machines |
| S4 | Patcher seats for every Windows programmer. Asset reload over the network for artists | An artist sees an edit on a remote build | Live servers |
| S5 | Server-pushed config and data with schema versions and rollback. This is a live-ops safety problem, not an iteration-speed one | Ops change tuning on live servers without a deploy | — |

Guidance Inference the report's synthesis

**Build vs buy.** Buy the watcher and, on Windows and consoles, the code patcher. DLL-swap is the only Linux option found. Build the debounce, frame-boundary apply and handle swap.

**Lock early.** Nothing irreversible, as long as gameplay sits behind a module boundary.

**Pitfalls.**

- Architecture contorted early for DLL-swap.
- Function pointers into unloaded modules.
- Static initializers in reloadable code.
- LTO builds, which Live++ can't use.
- Half-written files read on duplicate events.
- Reload paths that rot untested.

**Learn.** Shared-library loading and symbol export, ABI and struct layout, OS file-notification semantics.

Why it matters

It shortens iteration with nothing irreversible to lock, as long as gameplay sits behind a module boundary.

[Back to the category map](#category-map)

### Diagnostics

#### Debug tools and profiling

Logs, asserts, CVars and capture hooks on day one.

What it is

Debug tools are the day-one instruments: log categories, asserts, console variables, an in-game console, debug drawing and profiler hooks.

Key findings Established sourced facts, as observed in early November 2026

**Logging.** Unreal's log categories have seven verbosity levels (Fatal to VeryVerbose), changeable at runtime ([Unreal logging](https://dev.epicgames.com/documentation/en-us/unreal-engine/logging-in-unreal-engine)).

**Asserts** ([Unreal asserts](https://dev.epicgames.com/documentation/en-us/unreal-engine/asserts-in-unreal-engine)):

- `check` "only operate\[s\] in Debug and Development builds."
- "Verify macros evaluate their expressions even in builds where Check macros are disabled."
- On a failed `ensure`, "the Engine will inform the crash reporter, but will continue running," once per session by default.

**Debug drawing.** im3d "does not affect the system graphics state directly, therefore Im3d calls can be made from anywhere" ([im3d](https://github.com/john-chapman/im3d)).

**GPU capture (RenderDoc).**

- RenderDoc v1.47 (MIT) is current ([RenderDoc](https://github.com/baldurk/renderdoc/releases/tag/v1.47)).
- Its in-application API says "It is not recommended to actually link against RenderDoc's DLL." Detect it, resolve `RENDERDOC_GetAPI`, and fall back safely if it is absent.
- `UnloadCrashHandler` exists because RenderDoc's handler "could interfere" with your own ([RenderDoc API](https://renderdoc.org/docs/in_application_api.html)).

**Profiling (Tracy).**

- Tracy v0.14.1 is BSD-3 ([Tracy](https://github.com/wolfpld/tracy)).
- It ships a "headless capture-only utility" (`tracy-capture -s`), plus `tracy-csvexport` for per-zone mean, min, max and standard deviation ([Tracy manual](https://github.com/wolfpld/tracy/blob/master/manual/tracy.tex)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Log channels with verbosity and a file sink. The assert/verify/ensure trio. A CVar registry with flags. An ImGui console with history. A stat overlay (frame, CPU and GPU ms, memory by pool, draw calls) fed from the profiler's counters. Debug draw with per-category toggles | Every subsystem logs by category, and no raw `printf` or `assert` remains. Physics, AI, navigation and audio each draw their state on demand. The overlay costs under \~0.2 ms | The first public build |
| S2 | A RenderDoc hotkey or CVar capture with scene-named titles. `ensure` wired to the crash reporter. Dev-only CVars and commands stripped from shipping builds; logs and `verify` kept | One keypress yields a titled capture in a known folder. Shipping builds contain no cheat commands | Platform ports |
| S3 | Platform log sinks and per-platform stat overlays. Console capture tools (NDA) | Each platform's logs reach the same viewer | Several teams |
| S4 | Per-team log categories. CVar docs generated from the registry. Scripted profiling runs in CI, comparing per-zone stats against a committed budget | CI fails on a frame-time regression beyond budget | Live servers |
| S5 | An authenticated remote console. Server-side CVar replication. Audit logs. Structured logs shipped to a log store | Ops inspect and tune live servers safely | — |

Guidance Inference the report's synthesis

**Build vs buy.** Build logging, asserts, CVars and the console; they are small, and the policy is engine-specific. Buy im3d, RenderDoc and Tracy.

**Lock early.** Assert semantics and CVar flags.

**Pitfalls.**

- CVars as plain globals read across threads.
- `assert(sideEffect())` vanishing in release.
- `ensure` spam.
- Log I/O on the main thread.
- RenderDoc's crash handler shadowing yours.
- Profiler zones left on in shipping builds.

**Learn.** `std::format`, thread-safe sink queues, SEH and signal basics, ImGui, line-rendering shaders.

Why it matters

They are how every other subsystem's “done when” gate gets observed and measured.

[Back to the category map](#category-map)

#### Crash reporting

Out of process, symbolized, consented, before the first public build.

What it is

Crash reporting captures a dump when the game crashes, uploads it with consent, and matches it to debug symbols so it can be read.

Key findings Established sourced facts, as observed in early November 2026

**Capture mechanics.**

- `MiniDumpWriteDump` "should be called from a separate process if at all possible," because in-process calls risk loader deadlocks ([Microsoft Learn](https://learn.microsoft.com/en-us/windows/win32/api/minidumpapiset/nf-minidumpapiset-minidumpwritedump)).
- Crashpad's handler "runs in a separate process from the client." It uploads over the Breakpad protocol, throttled to "one upload per hour" ([Crashpad](https://chromium.googlesource.com/crashpad/crashpad/+/HEAD/doc/overview_design.md)).
- Sentry Native 0.17.5 (MIT) defaults to Crashpad on Windows, macOS and Linux. It has a `require_user_consent` option ([sentry-native](https://github.com/getsentry/sentry-native)).

**Services and pricing.**

| Service | Free tier | Paid tiers | Consoles |
| --- | --- | --- | --- |
| Sentry ([Sentry pricing](https://sentry.io/pricing/)) | 5k errors, 1 user | Team $26–29/month | SDKs "require separate access" ([Sentry docs](https://docs.sentry.io/platforms/native/)) |
| Backtrace ([Backtrace pricing](https://backtrace.io/pricing)) | 25k errors | — | Included at $1,200/user/yr |

**Steam's built-in reporter is not an option.** It is "nearing its End-Of-Lifetime" and "only supports 32-bit applications on Windows" ([Steamworks](https://partner.steamgames.com/doc/features/error_reporting)).

**Symbols and core dumps.**

- SymStore "does not support simultaneous transactions from multiple users," and public and private PDBs can't share a server ([SymStore](https://learn.microsoft.com/en-us/windows/win32/debug/using-symstore)).
- debuginfod serves Linux symbols by build ID ([debuginfod](https://sourceware.org/elfutils/Debuginfod.html)).
- systemd-coredump deletes cores "after a few days by default" ([systemd-coredump](https://www.freedesktop.org/software/systemd/man/latest/systemd-coredump.html)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | An SEH/signal handler that writes a log tail plus a local minidump or core from a dedicated thread or process. `ensure` logs non-fatal issues | A deliberate crash in dev produces a dump that opens symbolized | First public build |
| S2 | An out-of-process reporter (Crashpad or Sentry Native) feeding a backend. Build ID, version and channel embedded. A consent prompt. CI uploads symbols for every build that leaves the building. Crash grouping, release tags, alerts on new signatures. A Linux core-dump story | Test crashes from packaged builds on every OS appear symbolized within minutes. Each new top-10 crash has an owner within a day of a release | Console ports |
| S3 | Console crash SDKs under platform access | Console test builds report through the same triage loop | Team growth |
| S4 | A triage rotation. Symbol-store governance (one administrator). Crash-free-session metrics | Triage runs on a schedule | Server fleets |
| S5 | Server core dumps with symbol servers. Automatic restart with dump capture, correlated with server logs | Server crashes are triaged like client crashes | — |

Guidance Inference the report's synthesis

**Build vs buy.** Buy the capture and the backend. Build the build-ID stamping, consent UX, CI symbol upload and log-tail attachment.

**Lock early.** Build IDs embedded in every binary, and symbols archived for every public build.

**Pitfalls.**

- Dumps written in-process from the crashing thread.
- Missing symbols.
- Public and private PDBs mixed in one store.
- Upload throttling that hides launch-day crash storms.
- `RLIMIT_CORE` set to 0.
- Forgetting to ship the handler binary.
- No consent story.

**Learn.** SEH and POSIX signals, minidump and ELF core formats, PDB/DWARF and build IDs, WinDbg and gdb on dumps.

Why it matters

It must work before the first public build, because symbols for a shipped build cannot be recreated afterward.

[Back to the category map](#category-map)

#### Telemetry

Opt-in, minimal, and separate from crash consent.

What it is

Telemetry is data the game sends about sessions and performance, here strictly opt-in.

Key findings Established sourced facts, as observed in early November 2026

**EU consent rules (ePrivacy Art. 5(3), EDPB Guidelines 2/2023)** ([EDPB 2/2023](https://www.edpb.europa.eu/system/files/documents/2024-10/edpb_guidelines_202302_technical_scope_art_53_eprivacydirective_v2_en_0.pdf)):

- The rule applies when an entity instructs a device to send information, and "The notion of information includes both non-personal data and personal data."
- Client code that sends a unique identifier counts as "a 'gaining of access'."
- Such access needs "consent or necessity for specific purposes."

**GDPR scope.** Personal data includes "an IP address, a cookie ID, and the advertising identifier." GDPR also applies to non-EU organizations "offering goods/services ... to individuals in the EU" ([European Commission](https://commission.europa.eu/law/law-topic/data-protection/data-protection-explained_en)).

**Tooling.**

- GameAnalytics' free tier has "No MAU cap," but data export is "Not included" ([GameAnalytics](https://gameanalytics.com/pricing/)).
- Sentry caches crash envelopes while consent is revoked ([sentry.h](https://github.com/getsentry/sentry-native/blob/master/include/sentry.h)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Local files only: events, frame-time histograms and profiler captures from playtests, sent manually | A playtest yields a frame-time distribution per level and a progression log | Public release |
| S2 | First-launch consent, with separate toggles for crash reports and analytics. A random, resettable install ID (no hardware IDs). A minimal schema: session, build, coarse hardware class, perf histogram, milestones. Privacy-policy text. A deletion contact. Perf histograms by GPU/CPU class if hardware diversity bites. "No telemetry" is a valid choice for a single-player game | With consent off, a packet capture shows zero network calls. With consent on, events arrive and can be queried | Consoles |
| S3 | Platform-holder telemetry and privacy rules (NDA) | Each platform's rules are applied | Several titles share a pipeline |
| S4 | A versioned event-schema registry, a retention policy, a data map for privacy requests, dashboards per title | Adding an event requires a schema PR, and privacy requests can be answered | Live ops |
| S5 | Streaming ingestion, server-side gameplay events, experimentation | Live tuning decisions are backed by data within hours | — |

Guidance Inference the report's synthesis

**Build vs buy.** Build the client event queue and consent gating. Buy or self-host the backend.

**Lock early.** The consent model and the install-ID design.

**Pitfalls.**

- IPs or hardware serials as identifiers.
- One toggle covering both crash reports and analytics.
- Telemetry that blocks the frame.
- Events with no build ID or schema version.
- Collecting "everything."

**Learn.** GDPR and ePrivacy basics, event-schema design, histogram aggregation, HTTP batching and backoff. These notes are not legal advice.

Why it matters

Its consent model and install-ID design are legal commitments made at first release.

[Back to the category map](#category-map)

### Delivery

#### Automated testing

Five layers, each tied to a deterministic seam.

What it is

Automated testing spans five layers: unit tests, headless smoke tests, golden images, replay tests and performance budgets.

Key findings Established sourced facts, as observed in early November 2026

**Unit-test frameworks.**

- doctest v2.5.3 (MIT) claims to be "by far the fastest" to compile ([doctest](https://github.com/doctest/doctest)).
- Catch2 v3.16.1 is BSL-1.0 ([Catch2](https://github.com/catchorg/Catch2)).
- GoogleTest 1.18.0 requires C++17 ([GoogleTest](https://github.com/google/googletest)).
- Godot writes per-module doctest tests, and "The continuous integration system will fail if any compilation warnings are detected" ([Godot](https://docs.godotengine.org/uk/4.4/_sources/contributing/development/core_and_modules/unit_testing.rst.txt)).

**UI tests.** Dear ImGui Test Engine is free for entities with turnover "inferior to 2 million USD" ([license](https://github.com/ocornut/imgui_test_engine/blob/main/imgui_test_engine/LICENSE.txt)).

**Golden images (Unreal's screenshot comparison)** ([Unreal](https://dev.epicgames.com/documentation/en-us/unreal-engine/screenshot-comparison-tool-in-unreal-engine)):

- Ground truth is bucketed by "Platform_RHI_ShaderModel."
- Comparisons check both global and local error.
- Noisy features (TAA, motion blur, auto-exposure) can be disabled.

**Determinism and replay.**

- Box2D tests determinism in CI with a transform hash ([Box2D](https://box2d.org/posts/2024/08/determinism/)).
- L4D bots enabled stress testing "with accelerated time" ([Booth](https://steamcdn-a.akamaihd.net/apps/valve/2009/ai_systems_of_l4d_mike_booth.pdf)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Unit tests for math, containers, allocators, handles and serialization round-trips. Once a second level exists, a headless smoke test: boot, load every level, tick N frames, exit with a code | Unit tests run in under \~30 s on every push. Every level loads in CI on every desktop OS with no errors | The renderer stabilizes, or a perf target enters the release criteria |
| S2 | Smoke tests on every commit, plus nightly ASan/UBSan runs. Golden images for 5–10 scenes, with noise disabled, per-backend buckets, global and local thresholds, and an approval flow. Replay tests (inputs + seed + build → per-tick state hashes) if the sim is deterministic. Nightly perf budgets from scripted captures on dedicated hardware | A shader regression fails CI with a diff image. Bug repros arrive as replay files. A regression beyond tuned noise fails the nightly | Platform ports |
| S3 | Golden buckets per platform and backend. Smoke tests on devkit runners (NDA) | Every platform's smoke and golden tests run nightly | Team growth |
| S4 | UI and tool automation. Test ownership. Flaky-test quarantine | Core editor workflows have automated coverage | Servers at scale |
| S5 | Bot clients, soak tests, server load tests, network-condition matrices | Server capacity numbers come from repeatable tests | — |

Guidance Inference the report's synthesis

**Build vs buy.** Buy the unit framework. Build the smoke, golden, replay and perf harnesses, roughly 1–2k lines of engine-specific glue.

**Lock early.** Determinism (fixed tick, seeded RNG, no wall clock). Replay tests are impossible without it.

**Pitfalls.**

- Golden images on GPU-less CI runners.
- Perf tests on shared, noisy runners.
- TAA left on in golden tests.
- Replay tests on a non-deterministic sim.
- Tests written for systems still in flux.

**Learn.** CTest, deterministic simulation, image-diff statistics, perf-noise statistics (variance, percentiles).

Why it matters

Each layer depends on a deterministic seam built earlier, so testing shows whether those seams hold.

[Back to the category map](#category-map)

#### Build, CI and release

Scripts own the logic, CI only invokes them.

What it is

Build, CI and release cover compiling on every platform, continuous integration (CI), dependency pinning, version control and store uploads.

Key findings Established sourced facts, as observed in early November 2026

**Build configuration and dependencies.**

- CMake's latest stable is 4.4.5. `CMakePresets.json` can be committed, but "CMakeUserPresets.json should NOT be checked in." Workflow presets arrived in 3.25 ([CMake](https://github.com/Kitware/CMake/releases/tag/v4.4.5); [cmake-presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html)).
- vcpkg binary caching is "especially effective … in CI" ([vcpkg](https://learn.microsoft.com/en-us/vcpkg/consume/binary-caching-overview)).

**Compiler caches.**

- sccache v0.19.0 (Apache-2.0) supports GCC, Clang and MSVC with remote backends ([sccache](https://github.com/mozilla/sccache)).
- ccache is GPL-3.0+ ([ccache](https://github.com/ccache/ccache)).

**CI pricing.** GitHub cut hosted-runner prices "up to 39%" on January 1, 2026, and postponed the planned self-hosted charge ([GitHub](https://github.com/resources/insights/2026-pricing-changes-for-github-actions)).

**Large-binary storage.**

- **Git LFS:**
  - Git LFS's `lock` blocks concurrent edits of a path ([git-lfs-lock](https://github.com/git-lfs/git-lfs/blob/main/docs/man/git-lfs-lock.adoc)).
  - GitHub's free LFS quota is 10 GiB, with metered billing beyond it ([GitHub Docs](https://docs.github.com/en/billing/concepts/product-billing/git-lfs)).
- **Perforce P4** is "free for up to 5 users and 20 workspaces" ([Perforce](https://www.perforce.com/products/helix-core/free-version-control)).
- **Unity Version Control** offers unlimited seats with 25 GB free ([Unity](https://support.unity.com/hc/en-us/articles/34748492914964-Understanding-Unity-DevOps-charges)).

**Branching.** Trunk-based development cuts release branches "on a just-in-time basis" ([trunkbaseddevelopment.com](https://trunkbaseddevelopment.com/)).

**Steam uploads (SteamPipe)** ([Steamworks](https://partner.steamgames.com/doc/sdk/uploading)):

- Use "a dedicated build account."
- "The 'default' branch can not be set live automatically."
- After security changes to the build account, you wait 3 days before setting a released app's build live.

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | CMake targets per layer. Committed presets (dev, release and sanitizer, per OS). A pinned dependency manifest (vcpkg baseline or Conan lockfile) | A fresh clone builds on every desktop OS with one workflow command | Two OSes, or breaking a build you don't use daily |
| S2 | CI on every push for every OS, with warnings as errors, compiler and dependency caches, and symbols as artifacts. A version scheme (semantic version plus a monotonic build number). Packaging, cook and pack. Symbol upload. Store upload to a beta branch; the default branch is promoted by hand | CI runs under \~15 min warm. A red trunk is fixed or reverted the same day. A tagged commit produces a store beta build with no manual step except promotion | Platform ports |
| S3 | Toolchain files per target. Console SDKs (NDA). A per-platform cook and test matrix | Every platform builds nightly from the same commit | The first collaborator, or binaries beyond the free LFS quota |
| S4 | Binary-asset VCS with locks. Trunk with short-lived branches and required review/CI. Feature flags. One-command onboarding | A new teammate is productive on day one. Locks prevent binary conflicts | Build and cook times grow with headcount |
| S5 | Distributed compilation, a build farm, merge queues, parallel live branches and patch lines | Throughput stays flat as the team grows. Hotfix branches ship independently | — |

Guidance Inference the report's synthesis

**Build vs buy.** Buy everything. Keep the logic in CMake/Python scripts that CI merely invokes, so a 10-year engine isn't tied to one vendor's YAML dialect.

**Lock early.** Per-layer build targets with private links, which enforce the dependency rule. Also the version and build-ID scheme.

**Pitfalls.**

- Committing user presets.
- Unpinned dependencies.
- Caches keyed without the compiler version.
- CI clones burning LFS bandwidth.
- Self-hosted runners exposed to public PRs.
- SteamGuard prompts breaking unattended uploads.
- Forgetting the 3-day wait after security changes.
- Long-lived branches.

**Learn.** Modern CMake (targets, usage requirements, toolchains, presets), package-manager manifests, CI YAML and scripting, git and LFS internals, store partner tooling.

Why it matters

Per-layer build targets enforce the dependency rule, and the release script is the path every public build takes.

[Back to the category map](#category-map)

## CategoryOnline

Identity, authority and release logistics set the schedule for everything online, and several of them have lead times that must start before the code is ready.

### Multiplayer

#### Networking and hosting

A simulation seam, not a socket.

What it is

Networking covers the client/server model, replication (sending world state to clients), prediction, lag compensation and server hosting.

Key findings Established sourced facts, as observed in early November 2026

**The canonical client/server model.**

- **Authority and commands.** Valve's Yahn Bernier describes "a single, authoritative server that is responsible for running the main game logic." Clients send user commands and predict by re-running unacknowledged commands. One-shot effects play only on a command's first run ([Bernier](https://developer.valvesoftware.com/wiki/Latency_Compensating_Methods_in_Client/Server_In-game_Protocol_Design_and_Optimization)).
- **Prediction.** Gabriel Gambetta: prediction "requires a deterministic world," inputs carry sequence numbers, and irreversible outcomes wait for the server ([Gambetta](https://www.gabrielgambetta.com/client-side-prediction-server-reconciliation.html)).
- **Source engine defaults.** A **15 ms timestep**, a **100 ms interpolation delay**, and lag compensation by "Command Execution Time = Current Server Time - Packet Latency - Client View Interpolation" ([Valve SMN mirror](https://gist.github.com/CoolOppo/fe0586836de3fb2f90f9)).
- **Lockstep is fragile.** It must match "down to the bit-level," and "Floating point determinism is a complicated subject and there's no silver bullet" ([Fiedler](https://gafferongames.com/post/deterministic_lockstep/)).
- **Topology.** Unreal calls listen servers "convenient for casual cooperative and competitive multiplayer among small groups" ([Unreal](https://dev.epicgames.com/documentation/en-us/unreal-engine/networking-overview-for-unreal-engine)).

**Transport.**

- **GameNetworkingSockets** (BSD-3-Clause) provides reliable and unreliable messages, lanes, AES-GCM encryption, ICE-based P2P and fake lag/loss for testing. It explicitly excludes entity serialization and delta encoding ([GNS](https://github.com/ValveSoftware/GameNetworkingSockets)).
- **Steam Datagram Relay.** "The opensource code does not support accessing the relay network." On Steam, "Steam will take care of everything else" ([Steamworks SDR](https://partner.steamgames.com/doc/features/multiplayer/steamdatagramrelay)).

**Replication at scale.**

- **Bandwidth pipeline.** Fiedler's runs quantization → smallest-three quaternions → deltas against acked baselines → a priority accumulator ([State Synchronization](https://gafferongames.com/post/state_synchronization/)).
- **Relevancy.** In Unity's netcode, a ghost that becomes irrelevant arrives on the client as a destroy ([Unity](https://docs.unity3d.com/Packages/com.unity.netcode@1.6/manual/optimizations.html)).
- **Fortnite.** At **100 players and about 50,000 replicated actors**, per-connection relevancy "will bottleneck the server's CPU" ([Replication Graph](https://dev.epicgames.com/documentation/en-us/unreal-engine/replication-graph-in-unreal-engine)).
- **VALORANT.** Riot's move to push-model change tracking gave a "100x to 10000x performance improvement" in many cases ([Riot](https://technology.riotgames.com/node/112)).
- **EVE Online.** Time dilation keeps module response "under one second for the vast majority of the action" in fights of more than 1,300 pilots ([EVE](https://www.eveonline.com/article/time-dilation-hows-that-going)).

**Persistent backends: SpacetimeDB.**

- It is "a relational database that is also a server" ([SpacetimeDB](https://github.com/clockworklabs/SpacetimeDB)).
- Its BSL 1.1 license allows "no more than one SpacetimeDB instance in production" ([LICENSE](https://raw.githubusercontent.com/clockworklabs/SpacetimeDB/master/LICENSE.txt)).
- Standalone mode has no SSL ([docs](https://spacetimedb.com/docs)).

**Hosting operations.**

- **Godot** exports dedicated servers with a headless display server and recommends running them as a systemd service ([Godot](https://raw.githubusercontent.com/godotengine/godot-docs/master/tutorials/export/exporting_for_dedicated_servers.rst)).
- **systemd** supervision primitives are `Type=notify`, `Restart=on-failure` and `WatchdogSec` ([systemd.service](https://www.freedesktop.org/software/systemd/man/latest/systemd.service.html)).
- **Docker.** UDP ports must be published explicitly, and "Publishing container ports is insecure by default" ([Docker](https://docs.docker.com/engine/network/port-publishing/)).
- **Network testing.** `tc netem` adds jitter and burst loss ([tc-netem](https://man7.org/linux/man-pages/man8/tc-netem.8.html)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | The simulation seam (P3). The local player driven only by `InputCmd{seq, tick, buttons, axes, view}`. Replay from a saved state. A LAN direct-connect harness | Recorded commands replay to the same outcome. Results are identical with rendering off | Online play enters the design |
| S2 | One server model: simulation plus replication with zero or more local players. The listen host is that server plus a local client. Transport behind an interface (GNS or the store's networking). Server-assigned NetIds ↔ entities. Quantized serializers driven by reflection flags. Snapshots with deltas against acked baselines. Spawn/despawn. Remote entities interpolated \~100 ms behind. Local prediction that replays unacked inputs, with first-run-only FX. Lag compensation for hitscan. A version handshake. Auth tickets on join | 4 clients stay consistent at 100–150 ms RTT with 2–5% loss. Corrections are smooth, not snapping, under loss. Bandwidth is measured | More players, public matchmaking or dedicated servers |
| S3 | A headless dedicated build with no renderer, audio device or window. Service-manager supervision (readiness notify, restart on failure, watchdog). UDP ports published explicitly. NAT traversal through a relay. Cross-platform protocol versioning | The server survives crash-restart and watchdog tests. The full network-condition matrix passes | 8–16+ players, or a second networked title |
| S4 | A shared netcode module. Per-connection relevancy sets and a priority accumulator that fills a fixed bandwidth budget. Replication debug tools (bandwidth per entity). Bot clients | A new title replicates components by flags alone | 32–100 players, or persistent worlds |
| S5 | Spatial relevancy, dormancy and push-model change tracking. Multiple simulation processes with load-based assignment. Time dilation under overload. A persistent backend for accounts, progression and inventories, never for the tick | Capacity numbers come from repeatable load tests. Overload slows simulation time instead of dropping work | — |

Guidance Inference the report's synthesis

**Player count decides which replication layer you need (inference):**

| Players | Replication layer |
| --- | --- |
| \~4 | Full snapshots with quantization and acked deltas |
| 8–16 | Add per-connection relevancy and a priority accumulator |
| 32–100 | Add spatial relevancy, dormancy and push-model change tracking |
| MMO scale | Multiple simulation processes |

Designing replication around per-connection relevancy sets from the start keeps the whole path open.

**Build vs buy.** Buy the transport. Build replication, prediction and lag compensation. Treat persistent backends as stores for persistent state, and check their license limits before planning MMO scale.

**Lock early.** The fixed tick ([#2](#contract-2)), stable NetIds ([#3](#contract-3)), and the shape of the relevancy sets. Avoid lockstep unless you have made a determinism promise.

**Pitfalls.**

- Lockstep over floats.
- Replaying one-shot FX during prediction.
- Death FX driven by destroy messages, which irrelevancy also produces.
- A persistent database used as the tick server.
- Container ports exposed by default.
- Unencrypted transport left on outside development.

**Learn**, in order:

1. UDP and sockets.
2. Fix Your Timestep.
3. Gambetta's series and Bernier's paper.
4. Fiedler's protocol series and snapshot compression.
5. ICE and relay docs.
6. Linux service operations.
7. The Unreal, Riot and EVE scaling case studies.

Why it matters

It depends on the fixed tick and stable IDs, and player count decides which replication layer is needed.

[Back to the category map](#category-map)

#### Anti-cheat and security

Authority first, client anti-cheat last.

What it is

Anti-cheat and security cover who is trusted to change game state, plus optional client-side anti-cheat software.

Key findings Established sourced facts, as observed in early November 2026

**Client anti-cheat options.**

- **EAC** "is available for free through Epic Online Services on all PC platforms," and Proton support is enabled "with just a few clicks" in the EOS portal ([EOS news](https://onlineservices.epicgames.com/news/epic-online-services-launches-anti-cheat-support-for-linux-mac-and-steam-deck)).
- **BattlEye** on Proton needs only a request to BattlEye, according to Valve as reported by NME ([NME](https://www.nme.com/news/valve-says-steam-deck-devs-can-contact-battleye-to-enable-anti-cheat-3089079)).

**Linux and Deck compatibility risk.**

- Steam's Deck review lists "some anti-cheat providers" among the main causes of Unsupported ratings ([Steam Deck compatibility](https://partner.steamgames.com/doc/steamdeck/compat)).
- EA blocked Linux in Apex Legends because "There is currently no reliable way for us to differentiate a legitimate Steam Deck from a malicious cheat" ([EA](https://forums.ea.com/blog/apex-legends-game-info-hub-en/dev-team-update-linux--anti-cheat/9468053)).

**VAC** scans only "users connected to a secure server," and "VAC depends on the game server to exclude VAC banned users" ([VAC](https://partner.steamgames.com/doc/features/anticheat/vac_integration?l=romanian)).

**The Steam DRM wrapper** is "not an anti-piracy solution" and "is easily removed by a motivated attacker" ([Steam DRM](https://partner.steamgames.com/doc/features/drm)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Debug and cheat commands gated | Dev commands are compiled out or locked in shipping builds | External builds |
| S2 | Shipping hardening: no debug console, checksums on saves (tamper-evident, not secure), auth tickets for co-op joins | Joining validates the peer's ticket. Shipping builds contain no dev commands | Public matchmaking or competitive leaderboards |
| S3 | Trusted leaderboards and server-set stats. Evaluate EAC through EOS and confirm Proton/Deck impact | Anti-cheat on/off is verified on Windows, native Linux and Proton | Strangers playing at scale |
| S4 | Reporting and ban tooling shared across titles. Consented telemetry for anomaly detection | Bans are issued and appealed through one tool | A persistent economy |
| S5 | Authoritative servers own all valuable state. Client anti-cheat is one extra layer, with a deliberate Linux decision. Incident response | No client message creates value without server validation | — |

Guidance Inference the report's synthesis

**Build vs buy.** Build authority and validation. Buy client anti-cheat only where strangers compete.

**Lock early.** Where authority lives ([#16](#contract-16)). Adding anti-cheat later is easier than removing it.

**Pitfalls.**

- Kernel anti-cheat in a co-op game, which loses Deck and Linux players.
- Trusting client-reported stats for anything competitive.
- Treating VAC as drop-in.
- Using DRM as anti-tamper.

**Learn.** Threat modelling, authority and replication, store auth tickets.

Why it matters

Where authority lives (contract [#16](#contract-16)) is the real protection; client anti-cheat is a late and optional layer.

[Back to the category map](#category-map)

### Distribution

#### Platform services

One async interface, many optional backends.

What it is

Platform services are the store and platform features a game calls: identity, achievements, lobbies, cloud storage, overlay and voice.

Key findings Established sourced facts, as observed in early November 2026

**Steamworks startup and callbacks.**

- `SteamAPI_Init` "MUST be called and return successfully prior to accessing any of the Steamworks Interfaces!"
- `steam_appid.txt` is dev-only: "You should not ship this with your builds."
- `SteamAPI_RestartAppIfNecessary` should be "the first Steamworks function call you make."
- Call `SteamAPI_RunCallbacks` "at least once a second." Listeners run on the calling thread ([Steamworks API](https://partner.steamgames.com/doc/sdk/api)).
- Godot loads the Steam library dynamically and falls back from `SteamAPI_InitFlat` to legacy init ([Godot steam_tracker.cpp](https://cocalc.com/github/godotengine/godot/blob/master/main/steam_tracker.cpp)).

**Steam overlay.**

- "make sure to call SteamAPI_Init prior to initializing the OpenGL/D3D device."
- The overlay "requires a game consistently render frames, not pausing rendering" ([Steam Overlay](https://partner.steamgames.com/doc/features/overlay)).

**SDK 1.65 (July 2026; secondary source only).** It removed `IsRunningOnSteamDeck`, which developers had misused to tie graphics settings to hardware ([comss.ru](https://www.comss.ru/page.php?id=21392)).

**Stats, achievements and leaderboards.**

- Games start at 100 achievements.
- Stats set by game servers "cannot be set by clients" ([Steam Stats](https://partner.steamgames.com/doc/features/achievements)).
- Trusted leaderboards accept writes only through the Web API ([Steam Leaderboards](https://partner.steamgames.com/doc/features/leaderboards)).

**Release logistics.**

- Steam Direct costs **$100** per product.
- Paperwork takes "between 15-30 days."
- There is "A 30-day waiting period" after paying, a coming-soon page of at least two weeks, and a 1–5 day review ([Steam Direct](https://partner.steamgames.com/steamdirect)).

**Reference abstraction.** Unreal's Online Subsystem loads a default backend from config. Its interfaces may be null or "may simply return false," and async operations always call their delegates ([UE OSS](https://dev.epicgames.com/documentation/en-us/unreal-engine/online-subsystem-in-unreal-engine)).

**Other backends.**

- **EOS** is "free to use: no royalty or hosting fees." It works with custom engines and doesn't require shipping on the Epic Games Store ([EOS](https://onlineservices.epicgames.com/llms.txt)).
- **GOG's SDK** works offline, and "The GOG GALAXY client should not be required to launch the game" ([GOG](https://docs.gog.com/sdk/)).

**Online services.**

- **Steam lobbies** hold up to 250 users. "Only the lobby owner can set or delete the lobby data," and invites can cold-start the game with `+connect_lobby` ([Steam Matchmaking](https://partner.steamgames.com/doc/features/multiplayer/matchmaking)).
- **Vivox voice** is free "Up to 5000 PCU," then $2,000 per 5,000-PCU block at the first tier ([UGS pricing](https://unity.com/products/gaming-services/pricing)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | An `IPlatformServices` set of capability interfaces: identity, entitlements, achievements/stats, leaderboards, presence, sessions/lobbies, user storage, UGC, overlay/external UI, voice. All async, each optional. A Null backend that logs unlocks locally. Optional dynamic loading of the store library | No platform header is included outside the platform module. The game runs with the store absent, present, or failing init | A decision to sell on a store. Start the paperwork now, because the mandatory waits run to weeks |
| S2 | One store backend. Restart-if-necessary, then init, both before GPU device creation. Callbacks pumped once per frame on the main thread. Stats stored at checkpoints, plus an in-game achievement view. Localized rich presence. An overlay pause hook that keeps rendering. DLC checks. A store playtest app. Native Linux builds on a pinned runtime. If multiplayer: lobbies, invites and cold-start join. `PlayerId` as an opaque, platform-tagged value | The overlay opens over every render backend and in menus. Achievements unlock offline and sync later. The dev app-ID file is excluded from depots. Invites join both from a cold start and in-game | A second store, a console, or cross-play |
| S3 | A second store, or a cross-store service (EOS for identity, lobbies, voice and anti-cheat), plus console backends. Multi-local-user identity. Per-platform achievement and stat IDs mapped in data. A capability matrix per platform | The same binary selects backends by config. A Steam player and a non-Steam player join one lobby. Voice respects mute, block and platform privacy | A second title wants the layer |
| S4 | The platform layer as a versioned module with conformance tests per backend. Per-title configuration as data. Ops dashboards | A new backend passes the shared suite (unlock, store, presence, lobby create/join). A new title integrates achievements by data only | Persistent online services |
| S5 | A first-party account service as canonical identity, with platform accounts linked to it. Server-set stats and trusted leaderboards. Server-side ownership checks. Voice at scale with a cost model | Accounts link and unlink across platforms. No client can write a progression stat. Voice cost is modelled against projected PCU | — |

Guidance Inference the report's synthesis

**Build vs buy.** Buy the store SDKs and EOS. Buy voice from EOS or a PCU-priced vendor. Build the abstraction.

**Lock early.** Contracts [#16](#contract-16) and [#17](#contract-17). Design the abstraction around async calls and optional capabilities, not around one store's shapes such as 64-bit IDs or synchronous "is unlocked" checks.

**Pitfalls.**

- Shipping the dev app-ID file.
- Creating the GPU device before store init.
- Stopping rendering on pause.
- Pumping callbacks from a worker thread.
- Gating graphics presets on hardware detection.
- Store IDs as primary keys in persistent databases.
- Assuming members can write lobby data.
- No cold-start join handling.

**Learn.** C ABI and dynamic loading, async callback patterns, store build scripting (VDF, steamcmd), partner-site concepts, OAuth-style auth for account linking, voice pipeline basics.

Why it matters

An async interface with optional capabilities (contracts [#16](#contract-16) and [#17](#contract-17)) keeps the game from being shaped around one store.

[Back to the category map](#category-map)

#### Patching, updates and DLC

Pack layout is a launch-day decision.

What it is

Patching, updates and DLC cover how changed content reaches players and how add-on content is owned and mounted.

Key findings Established sourced facts, as observed in early November 2026

**SteamPipe** ([SteamPipe](https://partner.steamgames.com/doc/sdk/uploading)):

- Builds go live on branches, and users "will be able to download this update within a couple of minutes."
- Chunks are about 1 MB, and an absolute-offset change costs a whole new chunk.
- `FileExclusion` keeps files out of depots.
- `userconfig` files are never overwritten.
- DLC can live in depots that "only be downloaded by users who own the content."

**Steam DLC rules** ([Steam DLC](https://partner.steamgames.com/doc/store/application/dlc)):

- Each DLC has its own app ID, checked with `BIsDlcInstalled`.
- Secure online games verify ownership server-side with `CheckAppOwnership`.
- Valve advises against in-game currency in DLC, and says day-1 DLC "might not be a good idea."

**Xbox requirements** ([Xbox Requirements](https://learn.microsoft.com/id-id/gaming/gdk/_content/gc/policies/console/certification-requirements)):

- Content updates must load older saves (XR-003-16).
- Add-ons "must not be required for users to complete any of the main features" (XR-037).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Loose files or a simple pack. Deterministic cook | Two cooks of unchanged content are byte-identical | First external build |
| S2 | Packs of 1–2 GB or less, grouped by level or feature. Assets aligned to 1 MB or appended only. Per-asset compression only. Scripted uploads to default, beta and password branches. Settings files marked as user config | A one-asset change yields a patch of a few MB, verified with a preview build. Old saves load in the new build in an automated test | First DLC, or a console |
| S3 | `IEntitlements` per store. DLC as separate mounts. Saves degrade gracefully when their DLC is missing | A DLC-owning save opened on a non-owning account degrades gracefully | Frequent live updates |
| S4 | Reproducible builds in CI. Automated patch-size regression checks. Release branches | Patch size is tracked per build, and alerts fire on regressions | Live-service cadence |
| S5 | Server-validated entitlements. Client/server version negotiation. Staged rollouts via branches | The server refuses unowned content and handles mixed client versions | — |

Guidance Inference the report's synthesis

**Build vs buy.** The store provides delivery. Build the pack layout and the entitlement interface.

**Lock early.** Contract [#6](#contract-6), before the first public build.

**Pitfalls.**

- Absolute-offset TOCs.
- Whole-pack compression or encryption.
- Non-deterministic cooks, which turn every update into a full re-download.
- Reordering assets after launch.
- DLC the base game can't tolerate being absent.

**Learn.** Binary format design, chunked storage, build determinism, store build scripting.

Why it matters

Patch size is decided by the pack layout chosen at launch (contract [#6](#contract-6)).

[Back to the category map](#category-map)

#### Modding and UGC

Data-first content with a versioned schema, never native-code mods.

What it is

Modding and UGC (user-generated content) cover how players add content: data overrides, mod manifests, Workshop distribution and sandboxed scripts.

Key findings Established sourced facts, as observed in early November 2026

**Steam Workshop.**

- **Item types.** Ready-to-use items need no curation. Curated items sold in-game need "a persistent item storage service" ([Steam Workshop](https://partner.steamgames.com/doc/features/workshop)).
- **Updates and uploads.** Workshop "handles keeping the client up to date with any mod updates." Users on temporary licenses can't upload.
- **Launch requirements.** A header image, a description, tool documentation and at least one published item.

**Sandboxing scripted mods (Lua community guidance)** ([lua-users wiki](http://lua-users.org/wiki/SandBoxes)):

- Start "by trusting nothing."
- Exclude `load`/`loadstring`/`dofile`, `debug`, `io` and most of `os`.
- Reject bytecode.
- A basic sandbox "doesn't handle resource exhaustion issues."

**Console UGC rules.** Xbox's XR-018 requires titles to "Respect player UGC settings and gracefully handle scenarios in which a user doesn't have access to UGC" ([Xbox Requirements](https://learn.microsoft.com/id-id/gaming/gdk/_content/gc/policies/console/certification-requirements)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Data-driven content. A loose-file override folder for development. Hot reload | Designers change content without recompiling | A plan to support mods publicly |
| S2 | An official mods folder. Each mod has a manifest (ID, version, schema version, dependencies, load order) and mounts as a VFS overlay. No code mods. Optional ready-to-use Workshop items | Mods load deterministically in a given order. A mod with an incompatible schema is rejected with a message. The game boots clean with all mods disabled (safe mode) | Demand for scripting, or Workshop at scale |
| S3 | Workshop integration through `IUGC`. Per-platform UGC privilege checks. Mod lists recorded in saves and in multiplayer handshakes | Joining a session with mismatched mods produces a clear error. Console builds respect UGC privileges | Multiple titles |
| S4 | A mod SDK (exporters, validators). Sandboxed scripting with CPU and memory quotas. Content-hash compatibility checks | A third party builds a mod from public docs alone | Server-hosted UGC |
| S5 | Server-side validation and moderation. Curated or paid items through an inventory service | No client-authored UGC affects authoritative state without validation | — |

Guidance Inference the report's synthesis

**Build vs buy.** Buy distribution (Workshop or similar). Build the manifest, overlay rules, schema versioning and sandbox.

**Lock early.** Contract [#18](#contract-18): allow no native-code mods. Version the data schema (the save rules apply here too). Define load order explicitly.

**Pitfalls.**

- Native DLL mods.
- Unversioned data formats.
- Load order that depends on filesystem enumeration.
- Saves that reference mod content without recording the mod list.
- Exposing `load` or `debug` to scripts.

**Learn.** VFS mount stacks, schema versioning, sandboxing, the Workshop APIs.

Why it matters

The no-native-code rule (contract [#18](#contract-18)) and a versioned data schema keep mods from becoming a security and save-compatibility problem.

[Back to the category map](#category-map)

### Compliance

#### Console porting and certification

Business access first, then nine engine behaviors.

What it is

Console porting and certification cover platform access, the port itself and the platform holder's release requirements.

Key findings Established sourced facts, as observed in early November 2026

**Xbox.**

- **Program terms.** ID@Xbox requires developers to be at least 18 and to "sign an NDA." PlayFab services are listed as free when publishing on Xbox ([Microsoft Game Dev](https://developer.microsoft.com/games/publish)).
- **Requirements document.** The Xbox Requirements (v16.7, 10/27/2026) are public ([Xbox Requirements](https://learn.microsoft.com/id-id/gaming/gdk/_content/gc/policies/console/certification-requirements)). They cover:
  - suspend/resume without a full restart (XR-001-02)
  - no crash or hang when network services are slow or intermittent (XR-074)
  - account linking (XR-013)
  - never displaying another user's personal information (XR-014)
  - UGC privileges (XR-018)
  - add-on independence (XR-037)
  - user/controller pairing on resume (XR-112)
  - reestablishing an active controller (XR-115)
  - saves across console types (XR-130)
- **Static screens and loading (XR-001).** It fails "A non-interactive pause or static screen ... lasting over twenty seconds," or a loading screen over two minutes without a progress indicator ([XR-001](https://devdocs.xbox.com/publishing/certification/xr/xr-001.md)).

**PlayStation.** Newly accepted partners can receive one complimentary dev kit and one test kit ([SIE](https://sonyinteractive.com/en/?p=3002341)). Its technical requirements are not public.

**Nintendo.** Switch development needs a separate access request ([Nintendo Developer Portal](https://developer.nintendo.com/register)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | Nothing console-specific. No globals that assume one user or controller | — | — |
| S2 | PC equivalents: pause on controller disconnect, pause on focus loss, no hard stalls, loading progress UI, per-user save paths | The PC build passes a self-audit against the public XRs that apply to PC | A publisher or platform deal |
| S3 | Apply to the platform programs and get SDK access. Port the platform layer, render backend and audio backend, or contract a porting house | A pre-cert self-test against the XR list and NDA checklists passes. Suspend/resume soak tests are clean | Multiple titles on consoles |
| S4 | Console backends in CI on devkits. A cert-checklist tracker per platform | First-submission cert passes become routine | Online services on console |
| S5 | Platform-service rate limits, communication and privilege rules, cross-network play privileges | — | — |

Guidance Inference the report's synthesis

**Engine behaviors (inference).** The public XRs imply nine behaviors any console port needs:

1. A lifecycle state machine.
2. N local users with controller associations.
3. Controller-loss pause and re-pair UI.
4. Connectivity-loss handling on every online call.
5. Save compatibility across patches and DLC removal.
6. Privilege checks before multiplayer, communication and UGC.
7. Platform terminology tables.
8. Load-screen progress and no long static stalls.
9. Service rate limiting.

**Build vs buy.** Porting houses are an alternative when a team lacks platform licenses. No primary source gave their terms.

**Lock early.** Multi-user identity and the lifecycle state machine. Both are cheap to design for at S1 and expensive to retrofit.

**Pitfalls.**

- Assuming one signed-in user.
- Long non-interactive stalls.
- Online calls that hang on network loss.
- Saves broken by patches.

**Learn.** The public Xbox Requirements as a template. Everything else depends on platform-holder NDAs.

Why it matters

Access is a business step with long lead times, and a few engine behaviors are cheap at stage 1 and expensive to retrofit.

[Back to the category map](#category-map)

#### Legal, licensing and privacy

Automate notices from the first dependency.

What it is

This covers the legal plumbing a shipped game needs: third-party license notices, age ratings, and privacy obligations such as GDPR and COPPA.

Key findings Established sourced facts, as observed in early November 2026

**Library licenses.**

- LGPLv3 §4 requires prominent notice, a copy of the GPL and LGPL, and either relinkable objects or "a suitable shared library mechanism" ([LGPL-3.0](https://spdx.org/licenses/preview/LGPL-3.0.html)).
- Apache-2.0 carries attribution through a NOTICE file ([ASF](https://apache.googlesource.com/www-site/+/HEAD/content/legal/apply-license.md)).

**Age ratings.** On Steam, "all games must have an age rating in order to be offered to German customers," and unrated games have been hidden since November 15, 2024 ([Steamworks](https://partner.steamgames.com/doc/gettingstarted/contentsurvey/germany)).

**GDPR.** Requests get a response "in principle within 1 month," "free of charge" ([European Commission](https://commission.europa.eu/law/law-topic/data-protection/information-business-and-organisations/dealing-individuals-requests_mt)).

**COPPA.** Collecting information "that establishes that a visitor is under 13 triggers COPPA compliance" ([FTC](https://www.ftc.gov/node/60359)).

Stage ladder Inference the report's synthesis

| Stage | Implement | Done when | Trigger to next |
| --- | --- | --- | --- |
| S1 | A license manifest started with the first third-party import (name, version, license, notice text, link mode) | Every dependency has a license and version recorded | First external build |
| S2 | A generated notices screen or file. LGPL libraries linked only dynamically. Apache NOTICEs concatenated. The store content survey completed. Crash reporting with consent. No birthdate collection | The shipping build includes all notices. The game is rated and visible in Germany | Personal data collection, or a second platform |
| S3 | Ratings per console store. A privacy policy per platform. Platform terminology | Ratings obtained for every storefront. A privacy policy is linked in-game | Accounts or telemetry across titles |
| S4 | Central privacy tooling (data inventory, retention). Automated license scanning in CI | A dependency without a license entry fails CI | First-party accounts |
| S5 | A GDPR request pipeline (access, export, erasure within one month). Age-gate design and a COPPA consent flow if under-13 users are possible. Moderation and report tooling | An erasure request removes data across all services by policy | — |

Guidance Inference the report's synthesis

**Build vs buy.** Build the manifest and notices generator. Have a lawyer review the policies; these notes are not legal advice.

**Lock early.** Contract [#18](#contract-18), the LGPL link policy, before the first ship.

**Pitfalls.**

- Statically linking LGPL code in a shipped build.
- Missing NOTICE propagation.
- Collecting an age before COPPA handling exists.
- Showing other users' personal information.

**Learn.** Open-source license obligations, GDPR and COPPA basics, store rating processes.

Why it matters

License and privacy duties attach at first ship, and automating them from the first dependency avoids an audit later.

[Back to the category map](#category-map)

## Connections

The roadmap is a sequence of contracts more than a sequence of features, and these links between categories are where that shows. Inference the report's synthesis, regrouped

- One spine feeds six systems

  A fixed tick, a reflection layer with field flags, and stable IDs are built once in P3 and P4. Saves, replication, replay tests, play-in-editor snapshots, rollback physics and AI debugging timelines all consume them.

  [Game loop and time](#loop) [Serialization and saves](#serialization) [Entity and object model](#entities) [Networking and hosting](#networking) [Automated testing](#testing) [Editor](#editor) [AI, perception and navigation](#ai)
- Every contract is data that leaves the engine and comes back

  Assets go into packs, saves onto disk, commands onto the wire, patches through a store, settings across versions, strings to a translator, and identities to a platform holder. That return trip is what makes each of the eighteen contracts expensive to change.

  [Contracts](#contracts)
- Determinism is a chain with one weakest link

  Math conventions, the engine-owned random generator, procedural layout hashes, physics stepping, replay tests and network prediction each assume the link before them is deterministic. One unordered hash-map iteration, standard-library distribution or fused multiply-add breaks everything downstream.

  [Math](#math) [Random numbers](#rng) [Procedural generation](#procgen) [Physics and collision](#physics) [Automated testing](#testing) [Networking and hosting](#networking)
- Pack layout decides patch size

  A deterministic cook, a stable asset order, per-asset compression and a fixed table-of-contents position all exist so that a one-asset change produces a one-asset patch. DLC mounts and mod overlays reuse the same mount rules.

  [Asset pipeline and resources](#assets) [VFS and archives](#vfs) [Compression](#compression) [Patching, updates and DLC](#patching) [Modding and UGC](#modding)
- Procedural levels constrain rendering

  Levels assembled at runtime rule out whole-level lightmaps, a baked visibility set and level-wide probe placement. The alternatives are bakes per template, or dynamic lighting with shadow caching.

  [Procedural generation](#procgen) [Lighting and shadows](#lighting) [Terrain, LOD and occlusion](#terrain)
- Gameplay sound is separate from audible sound

  AI hears gameplay noise events, not the mixer. That keeps AI identical on a muted client and on a headless server that links no audio code.

  [Audio](#audio) [AI, perception and navigation](#ai) [Networking and hosting](#networking)
- Action-based input is the root of four features

  Rebinding, accessibility options such as hold-to-toggle, Steam Input glyphs, and replay or network command streams all need gameplay that reads actions instead of devices.

  [Input](#input) [Accessibility](#accessibility) [Platform services](#platform-services) [Automated testing](#testing) [Networking and hosting](#networking)
- The no-single-user rule spans four layers

  The platform layer, input pairing, the platform-services player ID and console certification all assume several local users, each tied to a controller. A global “the user” breaks all four.

  [Platform layer](#platform-layer) [Input](#input) [Platform services](#platform-services) [Console porting and certification](#console)
- Consent gates three systems

  Crash reports, analytics and privacy requests each depend on a consent model with separate toggles, set at first launch.

  [Crash reporting](#crash) [Telemetry](#telemetry) [Legal, licensing and privacy](#legal)
- License policy narrows library choice

  The rule to link LGPL code only dynamically touches the audio, video and text options that carry that license, and it feeds the generated notices screen.

  [Audio](#audio) [Video playback](#video) [Text and fonts](#text) [Legal, licensing and privacy](#legal)
- The forward-or-deferred choice propagates

  The decal path, lit particles, transparency handling and the anti-aliasing options all follow from the lighting architecture.

  [Lighting and shadows](#lighting) [Particles and VFX](#particles) [Atmosphere and post-processing](#atmosphere)
- The job system has three early customers

  The physics library asks the host for a job system, the entity model wants parallel scheduling, and animation wants parallel sampling. The audio callback thread must never be a job worker.

  [Concurrency](#concurrency) [Physics and collision](#physics) [Entity and object model](#entities) [Animation](#animation) [Audio](#audio)
- Business lead times run ahead of the code

  Stage transitions are mostly business events: a store page, a platform deal, a first hire, a live service. Four waits must start early: Steam's 15 to 30 days of paperwork plus a 30-day wait, console NDAs and devkits, a symbol pipeline before the first public crash, and a 3-day build-account wait after security changes.

  [Growth stages](#growth-stages) [Platform services](#platform-services) [Console porting and certification](#console) [Crash reporting](#crash) [Build, CI and release](#build)

## Open questions

The review found 16 unknowns, 14 conflicting views and 27 missing topics. The stage gates themselves are untested, and the report leaves eight design decisions for its reader to make.

### Unknowns

These are questions the report cannot answer from its sources. “Narrowed” means this update found newer evidence that is still not a primary source. “Not rechecked” means the claim rests on a secondary source and was not verified again.

| Question | What the report says | Status | After this review |
| --- | --- | --- | --- |
| How long does each stage or phase take? | No source gives effort estimates. The report says to treat the stage gates as the plan and any dates as forecasts. | Open | Nothing in the report sizes any phase, for a team or for one person. |
| Are the stage gates and triggers right? | Every stage table, “done when” gate and trigger is synthesis. No source publishes a canonical roadmap or exit criteria. | Open | No project has been run against these gates. Treat them as hypotheses to adjust after each stage. |
| Is a subsystem's stage the same thing as the project's stage? | Project stages advance on business events such as a ship decision or a console deal. Subsystem ladders advance on technical triggers such as a profiler result. | Open | The report uses S1 to S5 for both and never says what to do when they diverge, for example concurrency reaching S3 work while the project is still in S1. |
| How well does the plan fit one developer? | Stage 4 begins at “more than a few programmers”, and many gates assume a growing team. | Open | The report does not mark which gates a solo developer can skip or shrink. |
| What do PlayStation and Nintendo require for certification? | Only Microsoft's Xbox Requirements are public. PlayStation and Nintendo rules, console memory budgets, console JIT policies and console telemetry rules sit behind NDAs. | Open | Answerable only after platform access. |
| Can LGPL libraries be shipped on consoles? | Contract [#18](#contract-18) says to link LGPL code dynamically, and the report offers OpenAL Soft, FFmpeg and FriBidi as options. | Open | The report never says whether the LGPL's relink or shared-library condition can be met on a closed console. It needs a lawyer's answer before an LGPL library enters a console plan. |
| What are FMOD's current license terms? | They could not be read from FMOD's own page. The only citation found was from 2017, giving an indie threshold under US$600k. | Answered | Answered on 14 November 2026 from the primary source. [FMOD's licensing page](https://fmod.com/licensing) lists a fee per game: free or $2,000 for Indie, $6,000 for Basic and $18,000 for Premium, each with lifetime distribution rights. The [FMOD Studio page](https://fmod.com/studio) gives the limits: Indie for budgets under $600k and free when company revenue is under $200k, Basic for budgets under $1.8 million, and Premium above that. |
| Do MSVC and Clang support C++26 reflection? | GCC 16.1 ships it behind a flag. MSVC support was not confirmed, and compilers spell annotation queries differently. | Narrowed | Clang support runs through [Bloomberg's experimental fork](https://github.com/bloomberg/clang-p2996), and [a late-April 2026 write-up](https://www.wholetomato.com/blog/cpp26-reflection-large-codebases) reports no public MSVC support and no date. Both are snapshots, so recheck before planning the stage 4 migration. |
| Which license does PhysX 5 carry now? | The changelog says Apache-2.0 from 5.11, while the repository README still shows BSD-3. | Open | This review found only BSD-3 statements, including [NVIDIA's release post](https://developer.nvidia.com/blog/open-source-simulation-expands-with-nvidia-physx-5-release), and could not confirm the Apache-2.0 change. Read the license file of the exact release before use. |
| What does Oodle cost outside Unreal? | It is commercial with evaluation by email. Its terms outside Unreal are unknown. | Open | No public terms turned up in this review. |
| Was Google's marl scheduler archived? | The report states it was archived on April 27, 2026. | Not rechecked | A search did not surface the archive notice. The stakes are low, because the report already recommends other schedulers. |
| Did Steamworks SDK 1.65 remove IsRunningOnSteamDeck? | The claim rests on a secondary source only. | Not rechecked | Not rechecked in this update. |
| Is Bink still licensed outside Unreal? | The claim rests on a secondary source only. | Not rechecked | Not rechecked in this update. |
| Do GPU vendors support ARB_bindless_texture? | The report marks vendor support as unverified. | Open | Matters only if a GL-first renderer wants bindless textures before it has an explicit backend. |
| Does SDL_GPU reach PlayStation? | PlayStation is not listed among its backends. | Open | Decides whether SDL_GPU can be the only render backend for a three-console plan. |
| What do porting houses charge, and on what terms? | No primary source gave their terms. | Open | Needed to compare an in-house port against a contracted one at stage 3. |

Decisions the report leaves open Inference this review's reading

The report compares options for each of these and stops short of a choice. Each is a contract or a lock-in, so each needs an owner and a date.

- **First render backend.** Four options are compared (OpenGL 4.6, SDL_GPU, Vulkan 1.3, D3D12) and none is chosen. [Rendering core](#rendering-core)
- **View transform.** PBR Neutral, ACES 2.0 or AgX. The choice must be locked before art production. [Atmosphere and post-processing](#atmosphere)
- **Determinism promise.** Same-binary, cross-platform, or none. Contract [#10](#contract-10) asks for it in writing. [Contracts](#contracts) Decided 14 Nov 2026 Minimal: same-binary at most, added only where a system such as multiplayer requires it (see [Conflicting views](#conflicts)).
- **Entity storage.** Sparse-set or archetype storage, and EnTT or flecs. [Entity and object model](#entities) Decided 14 Nov 2026 Both: flecs (archetype) in the engine and EnTT (sparse-set) in the games (see [Conflicting views](#conflicts)).
- **Scripting language.** Lua 5.5 with sol2, Luau, or no embedded language. [Scripting](#scripting)
- **Shipping UI framework.** Custom UI over Clay or Yoga, RmlUi, NoesisGUI or Ultralight. [UI](#ui)
- **Audio route.** A library engine behind a facade, or Wwise or FMOD for designer tooling and console reach. [Audio](#audio) To research Owner direction on 14 Nov 2026: look into an engine audio system modeled on FMOD's design, in place of adopting FMOD (see [Conflicting views](#conflicts)).
- **Indirect lighting for generated levels.** Bakes per template, or dynamic lighting with shadow caching. [Procedural generation](#procgen)

### Conflicting views

Rows marked “Sources disagree” are Contested: a named source takes the other side. Rows marked “Internal conflict” or “Unstated boundary” are Inference: they are this review's reading of the report's own text. Five further sequencing disputes are already listed under [Sequencing disputes](#disputes).

| Topic | Type | The report's position | The conflict |
| --- | --- | --- | --- |
| Fixed tick for all gameplay | Sources disagree | Contract [#2](#contract-2): no gameplay code reads wall-clock time or render delta time. One fixed tick drives everything gameplay reads ([Game loop and time](#loop)). | A mainstream engine documents the opposite default. [Unity's manual](https://docs.unity3d.com/2023.2/Documentation/Manual/TimeFrameManagement.html) runs game code once per rendered frame on a variable step scaled by delta time, and ties the fixed step mainly to physics. The report presents the fixed-tick-everything model as settled and does not weigh its cost, such as the display latency that interpolation adds.  Decided 14 Nov 2026 Gameplay and physics simulation run on their own tick, separate from the engine's frame loop, so gameplay features can be suspended during development. This sides with the report's fixed-tick model. Inference It is the report's simulation and presentation split, with the stage 2 separation of real time from game time pulled into stage 1. Suspending then means pausing game time or switching off individual simulation systems, while tools, UI and rendering keep running on real time. Gameplay and physics stay on the same simulation tick as each other (see [Game loop and time](#loop)). |
| Forward or deferred lighting | Sources disagree | “The sourced default is clustered forward with a depth pre-pass”, citing Filament and DOOM ([Lighting and shadows](#lighting)). | [Epic's documentation](https://dev.epicgames.com/documentation/en-us/unreal-engine/forward-shading-renderer-in-unreal-engine) says “By default, Unreal Engine uses a Deferred Renderer”, for versatility and feature access. The report also argues for forward partly on MSAA, while its own ambient-occlusion and post-processing choices lean on TAA.  Decided 14 Nov 2026 A hybrid rendering architecture, in place of a pure forward or a pure deferred renderer. Inference The usual hybrid shades opaque surfaces in a deferred pass and transparent surfaces in a forward pass. Both passes should read one clustered light list and one material model, so the two paths cannot drift apart in look. The layout of the deferred pass's buffers becomes a new decision to lock, and the report's stage 1 forward pass still serves as the starting point (see [Lighting and shadows](#lighting)). |
| Engine first or game first | Sources disagree | Game first: a phase is done when the game runs on it ([Sequencing disputes](#disputes)). | Two of the report's anchor sources, Gregory and Dragonfly, are engine-first, and the bring-up order follows their dependency sequence. The report states the disagreement but does not say how a game-first project should reorder P0 to P10.  Decided 14 Nov 2026 A minimal engine baseline first, then game-driven. The dependency order stays as a constraint, and the game chooses among the phases it unblocks (see [Sequencing disputes](#disputes)). |
| When the job system arrives | Sources disagree | Only when a profiler shows CPU-bound frames (P10). | Our Machinery argues for scheduling everything through jobs from boot. Inside the report, the default physics library asks the host for a job system at P5, and the master grid schedules an engine job API in S2. The stopgap is the library's sample job pool, which means two schedulers for a while.  Decided 14 Nov 2026 Foundational jobs: one engine job API from P2, which the physics library uses, so the second scheduler goes away (see [Sequencing disputes](#disputes)). |
| Native Linux or Proton | Sources disagree | Stage 2 of [Platform services](#platform-services) lists “native Linux builds on a pinned runtime” as a default. | [Valve's Steam Deck FAQ](https://partner.steamgames.com/doc/steamdeck/faq) says “Valve has no strong preference” between Proton and a native build. A Windows-only build that runs through Proton is an accepted route, so native Linux is a choice with a maintenance cost the report does not weigh.  Decided 14 Nov 2026 Native Linux, to support the server and multiplayer code. Inference The dedicated server is the part that must be native: a headless Linux binary with no window, GPU or audio. A native Linux client is a separate choice, and Proton stays available for it. Stage 1 stays on one operating system (see the one-OS row in this table), so Linux arrives with the server and multiplayer work. |
| How much determinism is required | Internal conflict | Contract [#10](#contract-10) treats determinism as a promise to choose “before networking or replays”: same-binary, cross-platform, or none. | Several gates assume the answer already. P3 in stage 1 requires that recorded commands replay to the same result. Stage 3 of [Math](#math), [Random numbers](#rng) and [Procedural generation](#procgen) requires matching output across compilers and platforms. The physics library documents determinism only for the same binary and call order. The server-authoritative snapshot model in [Networking and hosting](#networking) tolerates divergence, so the quoted claim that prediction needs a deterministic world is stronger than the chosen design needs.  Decided 14 Nov 2026 Minimal. There is no blanket determinism promise for gameplay. Determinism is added only where a system outside gameplay requires it, with multiplayer as the example. Inference In the terms of contract [#10](#contract-10) this is same-binary at most, with no cross-platform promise. The stage 3 cross-platform gates in Math, Random numbers and Procedural generation become optional. Server-authoritative snapshots need no bit-exact determinism. Lockstep or rollback netcode would, and it would need the whole gameplay simulation to be deterministic, so that choice has to be made before the netcode is built. |
| An ECS behind an Entity API seam | Internal conflict | A seam first and a library second; never return library view types through the API ([Entity and object model](#entities)). | The performance case the report cites for ECS comes from iterating contiguous storage through the library's own views, which the seam hides. Its migration source concludes a full ECS is rarely achievable in existing projects, and stage 4 allows an in-house store, which cuts against buying the leaves. Whether to adopt an ECS at all is debated, and the report cites mostly proponents.  Decided 14 Nov 2026 An ECS is adopted across both the engine and the games. The engine uses archetype storage (flecs), and the games use sparse-set storage (EnTT). Inference Two libraries mean two runtime entity ID spaces, so the report's PersistentId becomes the one identity that saves and replication use. Each component type should live in exactly one of the two worlds, and the bridge between them needs one fixed sync point per tick. The report's rule against leaking library types into gameplay code now applies to both libraries (see [Entity and object model](#entities)). |
| Who owns the character controller | Internal conflict | The section heading says to integrate a library and own the character controller ([Physics and collision](#physics)). | The body does the opposite. Stage 1 uses the library's kinematic character with defaults, stage 2 tunes it, and the build-vs-buy call lists the library's two character styles as a reason to buy.  Decided 14 Nov 2026 A hybrid controller. The engine owns the character controller, the player and other entities both use it, and it passes basic movement data into rigid bodies to detect states such as grounded, sliding and crouching. Inference This settles ownership on the heading's side: the engine owns the controller logic and the state detection, and the library supplies the bodies and the queries. One caution from the report's own evidence: the physics library's documentation says a rigid-body character is usually not the best solution for a player, and its kinematic character already handles stairs, floor sticking and crouching (see [Physics and collision](#physics)). |
| One view transform, or a switch | Internal conflict | Contract [#12](#contract-12) locks one view transform before art production ([Atmosphere and post-processing](#atmosphere)). | The stage 1 gate asks that switching tonemappers be a config flag, three candidates are left open, and one candidate outputs sRGB only while stage 3 adds HDR display output. |
| One operating system or several in stage 1 | Internal conflict | [Platform layer](#platform-layer) starts on one OS and treats a second desktop OS as the trigger for stage 2. | P0 asks for CI on every target desktop OS, and stage 1 of [Concurrency](#concurrency) asks for ThreadSanitizer builds on Linux CI because the tool does not list Windows. A Windows-first project therefore needs Linux in stage 1.  Decided 14 Nov 2026 One operating system in stage 1, with build infrastructure that can support several. This replaces an earlier note here that put Linux in stage 1. Inference Only one system is a supported target, so the native Linux server arrives with the server and multiplayer work. The build infrastructure is what makes that arrival cheap: build presets per system, OS calls kept inside the platform layer, and a CI setup that can add a system without being restructured. A compile-only Linux job would keep that infrastructure honest, and it would also allow the ThreadSanitizer builds the report asks for in stage 1. Which system is the one has not been stated yet. |
| GL-first on every desktop OS | Internal conflict | Stages 1 and 2 can stay GL-class with no loss for the systems in the report ([GL-class API limits](#gl-limits)). | The same section notes that macOS stops at OpenGL 4.1, which predates the compute shaders the stage 2 lighting and particle designs rely on. A GL-first engine therefore drops macOS until an explicit backend exists, and the report never says so.  Decided 14 Nov 2026 No macOS support. Inference This removes the conflict. The OpenGL 4.1 limit applies only to macOS, so with macOS out, the GL-first plan no longer drops a target platform. The macOS path under Missing topics is out of scope by the same decision. |
| Audio cost while one price is unknown | Internal conflict | “Cost is not the deciding factor”, because every audio option is effectively free at small scale ([Audio](#audio)). | The same section could not read FMOD's current terms. The conclusion rests on a price the report did not see.  Answered 14 Nov 2026 The price is now known. [FMOD's licensing page](https://fmod.com/licensing) lists a fee per game: free or $2,000 for Indie, $6,000 for Basic and $18,000 for Premium. Indie covers budgets under $600k and is free when company revenue is under $200k ([FMOD Studio page](https://fmod.com/studio)). The fee is per game, not per year, so the report's claim holds for a small project. To research Owner direction: look into building an engine audio system modeled on FMOD's design, in place of adopting FMOD. Inference That fits the report's own path: a library engine behind a facade, with a custom mixer able to replace it later. Modeling means reusing FMOD's concepts in the facade and the data format, not copying its code (see [Audio](#audio)). |
| What counts as a leaf | Unstated boundary | Buy the leaves, write the seams ([Buy-and-build split](#buy-build)). | The report also recommends building the clustered lighting lists and shadow atlas, a custom pack reader, possibly a custom audio mixer, owned containers at stage 3, an in-house entity store at stage 4 and a physics fork at stage 5. No test is given for when an algorithm stops being a leaf.  Decided 14 Nov 2026 A leaf is a library bought at the beginning, while the engine's own systems are still being created. It is a temporary system, used because it gives better support than what the engine has, in an area where owning the code is not yet required. Inference This supplies the missing test: a system stops being a leaf when owning it becomes a requirement. The builds the report recommends then read as planned replacements. Each leaf needs an engine-owned interface in front of it from the first day, or its replacement becomes a rewrite. |
| The order of the stages | Unstated boundary | S3 multi-platform, then S4 team and titles, then S5 online ([Growth stages](#growth-stages)). | The report itself says stage transitions are business events, and those need not arrive in this order. A co-op game needs online play (P9) before its first ship, and a team can grow before any port. |

### Missing topics

These are topics an engine roadmap normally has to settle and this report does not cover. Inference the gap is this review's judgment The evidence column gives keyword counts from a search of the full original text.

| Topic | What is missing, and why it matters | Evidence in the report |
| --- | --- | --- |
| Platforms |  |  |
| Mobile and web targets | “Multi-platform” in the report means desktop plus consoles. Android, iOS, browsers and WebGPU each change the render backend, input, memory budgets and store rules. Decided 14 Nov 2026 Not targeting mobile, so that half of this gap is out of scope. Web targets have not been addressed. | 0 mentions of mobile, Emscripten, WebGPU or a web build |
| A macOS path | macOS appears only as the OpenGL 4.1 limit and as a Metal backend inside SDL_GPU. There is no plan for when or how a Mac build appears. Decided 14 Nov 2026 No macOS support, so this gap is out of scope. | 0 mentions of MoltenVK; no macOS row in any stage table |
| VR and XR | Headsets change frame pacing, the camera model, input and comfort rules. Decided 14 Nov 2026 VR and XR are targeted after a future release and a preparation period, with support kept during development. Inference The gap stays open as research: the report does not yet say what must stay VR-ready from the start. | 0 mentions of VR or OpenXR |
| Rendering |  |  |
| Shader-compilation stutter and pipeline caching | Stage 3 adds Vulkan or D3D12, where pipeline state objects compile at first use. Epic documents that on D3D12 the driver compiles shaders as pipeline states are created, which can cause long first-launch loads and hitches, and Unreal precaches them for that reason ([Epic](https://dev.epicgames.com/documentation/en-us/unreal-engine/generating-a-state-object-database-for-advanced-shader-delivery)). | 0 mentions of pipeline cache, PSO or stutter |
| Shader permutations and cross-compilation | The report fixes one offline shader language and SPIR-V output, but not how variants are managed or how SPIR-V reaches GLSL or Metal targets. | 0 mentions of permutation, shader variant or SPIRV-Cross |
| GPU crashes and driver faults | Crash reporting covers CPU-side dumps only. A lost GPU device and driver-specific bugs need their own detection and reporting. | 0 mentions of device lost or GPU crash tooling |
| Display and graphics settings | Window modes, DPI scaling, multiple monitors, variable refresh rate and quality presets are what a PC player touches first. “Min-spec” is used as a gate three times and never defined. | 0 mentions of fullscreen, borderless, DPI, VRR or quality preset |
| Characters, vegetation and water | Morph targets, facial animation, cloth, hair, foliage and water are standard content needs with engine-side cost. | 0 mentions of blend shape, facial, cloth, hair or foliage |
| 2D games | The report is 3D throughout. Sprites, tilemaps and 2D lighting would need their own rows. | 0 mentions of sprite or tilemap; Box2D is the only 2D item |
| Gameplay |  |  |
| The game-flow layer | Nothing covers the state machine above the simulation (boot, menu, loading, play, pause) or the save experience: slots, autosave and checkpoints. | 0 mentions of game state, save slot or autosave |
| The line between engine and game code | Abilities, stats, inventory, quests and dialogue get passing mentions at most. They may be deliberately out of scope, but the report never draws the boundary. | 1 mention of dialogue; inventory only as a store service |
| External level editors | Between hand-edited text scenes (S1) and an in-engine editor (S2) there is a third route: author levels in an existing tool and import them. | 0 mentions of TrenchBroom or LDtk; Blender is never mentioned as an authoring tool |
| Online |  |  |
| Server orchestration and fleet cost | Stage 5 assumes server fleets but says nothing about allocating, scaling or paying for them, or about matchmaking as a service to build or buy. | 0 mentions of orchestration, Kubernetes, Agones or GameLift |
| Denial-of-service protection | Public dedicated servers and relays are attack targets from the first public match. | 0 mentions of DDoS |
| Rollback netcode | Fighting and fast action games use rollback instead of the server-snapshot model. The report names rollback only as a trigger and for physics state. | 0 mentions of GGPO; rollback is never described as a netcode model |
| Commerce and cross-progression | Stage 5 mentions a persistent economy, but in-game purchases, store commerce APIs and carrying progress across platforms are absent. | 0 mentions of purchase, monetization or cross-progression |
| Depth on the persistent backend | SpacetimeDB gets three facts. Missing are how its model fits the fixed tick and server authority, alternatives to compare, and its change license: the license text names AGPL v3 with a linking exception as the license it converts to ([license](https://feed.nuget.org/packages/SpacetimeDB.Runtime/2.8.0/License)). | 4 mentions of SpacetimeDB; 0 of comparable backends |
| Release |  |  |
| Code signing and installers | Unsigned Windows builds trigger reputation warnings and macOS builds need notarization. Runtime redistributables are part of packaging too. | 0 mentions of code signing, notarization, SmartScreen or installer |
| Stores other than Steam | Release logistics are given for Steam only. Epic Games Store, GOG, itch, the Microsoft Store and the Mac App Store each have their own review and build rules. | Lead times and upload steps listed for Steam alone |
| Backups and disaster recovery | Source control, the symbol store and any server database are single points of failure for a ten-year engine. | 0 mentions of backup or disaster recovery |
| Process |  |  |
| Effort, staffing and a solo cut | The report admits it has no effort estimates. It also has no minimum path for one developer and no view of which roles each stage needs. | Stated by the report; “solo” appears once |
| Language and standard choice | C++ is assumed throughout with no baseline standard, although the cited libraries need C++17 or C++20. There is no policy on exceptions and RTTI, no view on C++20 modules, and no comparison with other languages. | 0 mentions of Zig, C# or C++ modules |
| Code-quality practice | Sanitizers and tests are covered. Static analysis, a coding standard, an API deprecation policy for the versioned modules of stage 4, and engine documentation are not. | 0 mentions of static analysis, clang-tidy or coding standard |
| Upgrades and maintenance | Only physics has a dependency-upgrade policy. There is no plan for moving shipped titles to a new engine version or for retiring old formats. | Upgrade cadence appears in one subsystem |
| Legal |  |  |
| Accessibility law for chat | Stage 5 lists speech-to-text and text-to-speech chat as guideline items. In the United States they are a legal duty: the CVAA covers in-game text and voice chat, and the FCC's class waiver for video game software ended on 31 December 2018 ([Fenwick](https://www.fenwick.com/insights/publications/cvaa-compliance-waiver-for-video-games-set-to-expire-in-december-2018), [Frankfurt Kurnit](https://fkks.com/news/video-games-with-advanced-communications-services-must-now-be-accessible-to-players-with-disabilities)). | 0 mentions of CVAA |
| Online-safety and moderation law | Chat and user-generated content bring moderation and reporting duties in several jurisdictions. The report treats moderation as tooling only. | Moderation appears as a stage 5 tool, with no legal source |
| Monetization, export and IP | Loot-box rules, export controls on shipped encryption, trademarks, the end-user license, and contributor IP agreements for a growing team are absent. | 0 mentions of loot box, export control, trademark or CLA |

### Checks run for this update

The original cites about 390 sources. This update rechecked the handful below, chosen because they looked doubtful or sat on the report's own list of uncertainties. The remaining sourced facts were not verified again.

- Confirmed Gregory's *Game Engine Architecture* is in a 4th edition published as two volumes. [CRC Press listing](https://www.routledge.com/Game-Engine-Architecture-Two-Volume-Set/Gregory/p/book/9781041162599)
- Confirmed ICU 79 was released on 2026-10-22, and part of its MessageFormat 2 API remains a technology preview. [ICU project site](https://icu.unicode.org/)
- Confirmed SpacetimeDB's license allows no more than one instance in production under its additional use grant. [License text](https://feed.nuget.org/packages/SpacetimeDB.Runtime/2.8.0/License)
- Consistent GCC 16.1 ships C++26 reflection behind `-freflection`. [wrocpp compiler matrix](https://wrocpp.github.io/toolset/compiler-support/)
- Not confirmed PhysX moved to Apache-2.0 at 5.11. Only BSD-3 statements were found. [NVIDIA](https://developer.nvidia.com/blog/open-source-simulation-expands-with-nvidia-physx-5-release)
- Not confirmed Google's marl repository was archived on April 27, 2026. The search returned no archive notice.
- Confirmed FMOD's current pricing, checked on 14 November 2026 after an owner comment: a fee per game of free or $2,000, $6,000 or $18,000 by tier. [FMOD licensing](https://fmod.com/licensing)

### Changes made to the original

- Nine top-level sections became seven categories to fit the project's three-to-seven rule. The Data layer is now a topic inside Foundation, and the original introduction and conclusion were folded into the Summary, Connections and Open questions.
- Three categories were renamed to short nouns: Peer subsystems to Subsystems, Player I/O to Interface, and Online and platform to Online.
- One cross-reference was repaired. Stage 5 of Atmosphere and post-processing pointed to a “world content” section that does not exist; it now points to Terrain, LOD and occlusion, where the sky model lives.
- Blocks labeled as inference inside an Evidence list (the nine console engine behaviors) were moved under Guidance so the Established label stays accurate.
- Five owner decisions were recorded on 14 November 2026 under [Sequencing disputes](#disputes): logging before the window with a timestamp on every log call, audio after rendering, hot reload early alongside the logging system, a foundational job system, and a minimal engine baseline followed by game-driven engine work. The job-system decision overrides the report's call. It and the engine-baseline decision are also noted under Conflicting views. Eight more decisions were recorded the same day under Conflicting views: gameplay and physics simulation run on their own tick, separate from the engine's frame loop; the renderer uses a hybrid of deferred and forward shading; Linux is a native target for the server and multiplayer code; the determinism promise is minimal, added only where a system such as multiplayer requires it; an ECS is adopted in both the engine (flecs, archetype storage) and the games (EnTT, sparse-set storage); the engine owns a hybrid character controller that feeds rigid bodies; stage 1 targets one operating system, with build infrastructure that can support several; and macOS is not supported. FMOD's pricing was also confirmed from its own pages that day, and an owner request to research an FMOD-style engine audio system was noted. Scope decisions were recorded under Missing topics the same day: no mobile target, no macOS target, and VR and XR after a future release. The owner's definition of a leaf was recorded under Conflicting views. Each is marked “Decided” and is not part of the original text. A wrong definition popover on “Gregory's TOC” in the same table was removed.

## Sources

388 sources carried over from the original report, listed under the first category that cites them, plus 15 added in this review.

### Framework 34

- [Bevy](https://docs.rs/bevy/latest/bevy/asset/index.html) docs.rs
- [Bevy plugins](https://bevy.org/learn/quick-start/getting-started/plugins/) bevy.org
- [cereal](https://uscilab.github.io/cereal/serialization_functions.html) uscilab.github.io
- [Dragonfly checkpoint #1](https://web.cs.wpi.edu/~claypool/misc/dragonfly/book/pdfs/dev-checkpoint-1.pdf) web.cs.wpi.edu
- [EnTT core](https://github.com/skypjack/entt/wiki/Core-Functionalities) github.com
- [EnTT signals](https://github.com/skypjack/entt/blob/HEAD/docs/md/signal.md) github.com
- [EnTT wiki](https://github.com/skypjack/entt/wiki/Entity-Component-System) github.com
- [Fiedler](https://gafferongames.com/post/fix_your_timestep/) gafferongames.com
- [Fiedler](https://gafferongames.com/post/floating_point_determinism/) gafferongames.com
- [Filament](https://google.github.io/filament/Filament.html) google.github.io
- [FNA](https://fna-xna.github.io/docs/appendix/Appendix-B:-FNA-on-Consoles) fna-xna.github.io
- [Gambetta](https://www.gabrielgambetta.com/client-side-prediction-server-reconciliation.html) gabrielgambetta.com
- [gameenginebook.com TOC](https://www.gameenginebook.com/toc.html) gameenginebook.com
- [GDC Vault](https://gdcvault.com/play/1022187/Parallelizing-the-Naughty-Dog-Engin) gdcvault.com
- [Godot docs](https://docs.godotengine.org/en/stable/_sources/tutorials/performance/using_servers.rst.txt) docs.godotengine.org
- [Handmade Hero Episode Guide](https://guide.handmadehero.org/) guide.handmadehero.org
- [ICU](https://unicode-org.github.io/icu/userguide/format_parse/messages/) unicode-org.github.io
- [Jolt Architecture](https://github.com/jrouwe/JoltPhysics/blob/master/Docs/Architecture.md) github.com
- [Khronos](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glClipControl.xhtml) registry.khronos.org
- [LGPL-3.0](https://spdx.org/licenses/preview/LGPL-3.0.html) spdx.org
- [lua-users wiki](http://lua-users.org/wiki/SandBoxes) lua-users.org
- [Our Machinery](https://ruby0x1.github.io/machinery_blog_archive/post/fiber-based-job-system) ruby0x1.github.io
- [P2059R0](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p2059r0.pdf) open-std.org
- [Petrie, "Write Games, Not Engines"](https://geometrian.com/projects/blog/write_games_not_engines.html) geometrian.com
- [SDL3 GPU](https://wiki.libsdl.org/SDL3/CategoryGPU) wiki.libsdl.org
- [SDL_GetPrefPath](https://wiki.libsdl.org/SDL3/SDL_GetPrefPath) wiki.libsdl.org
- [Steam Input](https://partner.steamgames.com/doc/features/steam_controller/getting_started_for_devs) partner.steamgames.com
- [Steam Stats](https://partner.steamgames.com/doc/features/achievements) partner.steamgames.com
- [Steamworks](https://partner.steamgames.com/doc/sdk/uploading) partner.steamgames.com
- [Taylor Univ. COS370](https://gfx.cse.taylor.edu/courses/cos370-game//slides/02_Systems.md.html) gfx.cse.taylor.edu
- [UE Online Subsystem](https://dev.epicgames.com/documentation/en-us/unreal-engine/online-subsystem-in-unreal-engine) dev.epicgames.com
- [UMD CMSC425, paraphrasing Gregory](https://www.cs.umd.edu/class/spring2018/cmsc425/Lects/lect02-architecture.pdf) cs.umd.edu
- [Unreal Engine Modules](https://dev.epicgames.com/documentation/en-us/unreal-engine/unreal-engine-modules) dev.epicgames.com
- [Xbox Requirements](https://learn.microsoft.com/id-id/gaming/gdk/_content/gc/policies/console/certification-requirements) learn.microsoft.com

### Foundation 98

- [aarand](https://github.com/LTLA/aarand) github.com
- [Abseil](https://abseil.io/docs/cpp/guides/container) abseil.io
- [ACL](https://github.com/nfrechette/acl) github.com
- [Assimp](https://github.com/assimp/assimp) github.com
- [basis_universal](https://github.com/BinomialLLC/basis_universal) github.com
- [Bevy](https://docs.rs/crate/bevy/latest/source/examples/ecs/nondeterministic_system_order.rs) docs.rs
- [Bitsquid](https://bitsquid.blogspot.com/2011/09/managing-decoupling-part-4-id-lookup.html) bitsquid.blogspot.com
- [Cap'n Proto](https://capnproto.org/language.html) capnproto.org
- [cgltf](https://github.com/jkuhlmann/cgltf) github.com
- [Clang ASan](https://clang.llvm.org/docs/AddressSanitizer.html) clang.llvm.org
- [Clang TSA](https://clang.llvm.org/docs/ThreadSafetyAnalysis.html) clang.llvm.org
- [Clang TSan](https://clang.llvm.org/docs/ThreadSanitizer.html) clang.llvm.org
- [concurrentqueue](https://github.com/cameron314/concurrentqueue) github.com
- [cppreference](https://en.cppreference.com/w/cpp/header/memory_resource) en.cppreference.com
- [cppreference](https://en.cppreference.com/w/cpp/compiler_support/23) en.cppreference.com
- [cppreference](https://en.cppreference.com/w/cpp/chrono/steady_clock) en.cppreference.com
- [DirectX blog](https://devblogs.microsoft.com/directx/directstorage-1-4-release-adds-support-for-zstandard/) devblogs.microsoft.com
- [DirectXMath](https://github.com/microsoft/DirectXMath) github.com
- [enkiTS](https://github.com/dougbinks/enkiTS) github.com
- [EnTT](https://github.com/skypjack/entt/wiki/Resource-management) github.com
- [fastgltf](https://github.com/spnda/fastgltf) github.com
- [FiberTaskingLib](https://github.com/RichieSams/FiberTaskingLib) github.com
- [FlatBuffers](https://flatbuffers.dev/evolution/) flatbuffers.dev
- [Flecs](https://www.flecs.dev/flecs/Systems.html) flecs.dev
- [floooh](https://floooh.github.io/2018/06/17/handles-vs-pointers.html) floooh.github.io
- [fmt](https://github.com/fmtlib/fmt) github.com
- [fsync(2)](https://manpages.ubuntu.com/manpages/resolute/man2/fsync.2.html) manpages.ubuntu.com
- [Game Programming Patterns](https://gameprogrammingpatterns.com/game-loop.html) gameprogrammingpatterns.com
- [GDC Vault](https://gdcvault.com/play/1022164/Multithreading-the-Entire-Destiny) gdcvault.com
- [GLM](https://github.com/g-truc/glm) github.com
- [Go commit](https://go.googlesource.com/go/+/515e6a9b12dfe654c86cfd070ee5d6ac144fe116%5E%21/) go.googlesource.com
- [Godot docs](https://github.com/godotengine/godot-docs/blob/master/tutorials/io/saving_games.rst) github.com
- [Godot import](https://github.com/godotengine/godot-docs/blob/master/tutorials/assets_pipeline/import_process.rst) github.com
- [HandmadeMath](https://github.com/HandmadeMath/Handmade-Math) github.com
- [Intel](https://www.intel.com/content/www/us/en/developer/articles/guide/12th-gen-intel-core-processor-gamedev-guide.html) intel.com
- [isocpp](https://isocpp.org/blog/2026/04/gcc-16.1) isocpp.org
- [Jolt HelloWorld](https://github.com/jrouwe/JoltPhysics/blob/master/HelloWorld/HelloWorld.cpp) github.com
- [kernel.dk](https://kernel.dk/io_uring.pdf) kernel.dk
- [Khronos glTF](https://khronos.org/gltf) khronos.org
- [Khronos wiki](https://www.khronos.org/opengl/wiki/OpenGL_and_multithreading) khronos.org
- [KTX-Software](https://github.com/KhronosGroup/KTX-Software) github.com
- [libFuzzer](https://llvm.org/docs/LibFuzzer.html) llvm.org
- [libstdc++-cvs](https://gcc.gnu.org/pipermail/libstdc++-cvs/2026q1/044515.html) gcc.gnu.org
- [LZ4](https://github.com/lz4/lz4) github.com
- [marl](https://github.com/google/marl) github.com
- [Masmano et al. 2008](https://www.cs.york.ac.uk/rts/publications/Masmano2008.html) cs.york.ac.uk
- [meshoptimizer](https://github.com/zeux/meshoptimizer) github.com
- [Microsoft Learn](https://learn.microsoft.com/en-us/windows/win32/api/timeapi/nf-timeapi-timebeginperiod) learn.microsoft.com
- [Microsoft Learn](https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-createwaitabletimerexw) learn.microsoft.com
- [Microsoft Learn](https://learn.microsoft.com/en-us/windows/win32/api/ioringapi/nf-ioringapi-buildioringreadfile) learn.microsoft.com
- [Microsoft Learn](https://learn.microsoft.com/windows/win32/api/winbase/nf-winbase-replacefilew) learn.microsoft.com
- [N2271](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2271.html) open-std.org
- [nlohmann](https://json.nlohmann.me/features/binary_formats/) json.nlohmann.me
- [oneTBB](https://github.com/uxlfoundation/oneTBB/blob/master/RELEASE_NOTES.md) github.com
- [openmamba](https://src.openmamba.org/rpms/libphysfs/commit/630952b2efbbd10c4cc18abe6408b0b7064f57b8) src.openmamba.org
- [pcg-random.org](https://www.pcg-random.org/) pcg-random.org
- [pcg-random.org](https://pcg-random.org/posts/cpp-seeding-surprises.html) pcg-random.org
- [PhysicsFS](https://icculus.org/physfs/) icculus.org
- [prng.di.unimi.it](https://prng.di.unimi.it/) prng.di.unimi.it
- [Protobuf](https://protobuf.dev/programming-guides/proto3/) protobuf.dev
- [Quake cvar.h](https://github.com/id-Software/Quake/blob/master/WinQuake/cvar.h) github.com
- [Quill](https://github.com/odygrd/quill) github.com
- [RAD](https://www.radgametools.com/oodle.htm) radgametools.com
- [randomascii](https://randomascii.wordpress.com/2013/07/16/floating-point-determinism/) randomascii.wordpress.com
- [Recast](https://github.com/recastnavigation/recastnavigation/blob/HEAD/Docs/_2_BuildingAndIntegrating.md) github.com
- [rename(2)](https://manpages.ubuntu.com/manpages/resolute/man2/rename.2.html) manpages.ubuntu.com
- [sanitizers wiki](https://github.com/google/sanitizers/wiki/AddressSanitizerManualPoisoning) github.com
- [SDL wiki](https://wiki.libsdl.org/SDL3/SDL_CreateWindow) wiki.libsdl.org
- [SDL wiki](https://wiki.libsdl.org/SDL3/SDL_RunOnMainThread) wiki.libsdl.org
- [SDL wiki](https://wiki.libsdl.org/SDL3/SDL_DelayPrecise) wiki.libsdl.org
- [SDL3 CategoryStorage](https://wiki.libsdl.org/SDL3/CategoryStorage) wiki.libsdl.org
- [SDL3 main functions](https://wiki.libsdl.org/SDL3/README-main-functions) wiki.libsdl.org
- [SDL3 README-gdk](https://wiki.libsdl.org/SDL3/README-gdk/raw) wiki.libsdl.org
- [SDL3 README-switch](https://wiki.libsdl.org/SDL3/README-switch/raw) wiki.libsdl.org
- [Snapshot Compression](https://gafferongames.com/post/snapshot_compression/) gafferongames.com
- [spdlog](https://github.com/gabime/spdlog) github.com
- [Steamworks Cloud](https://partner.steamgames.com/doc/features/cloud) partner.steamgames.com
- [Sutter](https://herbsutter.com/2026/03/) herbsutter.com
- [Sutter](https://herbsutter.com/2025/06/) herbsutter.com
- [Taskflow](https://taskflow.github.io/taskflow/Releases.html) taskflow.github.io
- [The Truth](https://ruby0x1.github.io/machinery_blog_archive/post/the-story-behind-the-truth-designing-a-data-model/) ruby0x1.github.io
- [Tom Looman](https://tomlooman.com/unreal-engine-cpp-save-system/) tomlooman.com
- [toml++](https://github.com/marzer/tomlplusplus) github.com
- [Tracy manual](https://github.com/wolfpld/tracy/blob/master/manual/tracy.tex) github.com
- [ufbx](https://github.com/ufbx/ufbx) github.com
- [Unity docs](https://docs.unity3d.com/Packages/com.unity.netcode@1.5/manual/time-synchronization.html) docs.unity3d.com
- [unordered_dense](https://github.com/martinus/unordered_dense) github.com
- [Unreal blog](https://www.unrealengine.com/blog/oodle-now-free-to-use-in-unreal-engine-via-github) unrealengine.com
- [Unreal config](https://dev.epicgames.com/documentation/en-us/unreal-engine/configuration-files-in-unreal-engine) dev.epicgames.com
- [Unreal console variables](https://dev.epicgames.com/documentation/en-us/unreal-engine/console-variables-cplusplus-in-unreal-engine) dev.epicgames.com
- [Unreal DDC](https://dev.epicgames.com/documentation/unreal-engine/using-derived-data-cache-in-unreal-engine) dev.epicgames.com
- [utf8everywhere.org](https://utf8everywhere.org/) utf8everywhere.org
- [VMA](https://gpuopen-librariesandsdks.github.io/VulkanMemoryAllocator/html/quick_start.html) gpuopen-librariesandsdks.github.io
- [wrocpp](https://wrocpp.github.io/toolset/compiler-support/) wrocpp.github.io
- [XGameSave](https://learn.microsoft.com/en-us/gaming/gdk/docs/features/common/game-save/xgamesave) learn.microsoft.com
- [xxHash](https://github.com/Cyan4973/xxHash) github.com
- [Zen loader](https://dev.epicgames.com/documentation/en-us/unreal-engine/zen-loader-in-unreal-engine) dev.epicgames.com
- [zstd](https://facebook.github.io/zstd/) facebook.github.io

### Subsystems 85

- [AnandTech](https://home.anandtech.com/print/12894/apple-deprecates-opengl-across-all-oses) home.anandtech.com
- [Announcing Box3D](https://box2d.org/posts/2026/06/announcing-box3d/) box2d.org
- [ARB_bindless_texture](https://github.com/KhronosGroup/OpenGL-Registry/blob/main/extensions/ARB/ARB_bindless_texture.txt) github.com
- [Aristidou](https://www.andreasaristidou.com/FABRIK) andreasaristidou.com
- [Audiokinetic](https://www.audiokinetic.com/en/pricing) audiokinetic.com
- [Audiokinetic Q&A](https://audiokinetic.com/qa/6164/virtual-voices-bad-practice?show=6174) audiokinetic.com
- [AZDO](https://www.slideshare.net/slideshow/gl-efficiency/32590355) slideshare.net
- [Baraff](https://www.cs.cmu.edu/~baraff/sigcourse/) cs.cmu.edu
- [Bencina](http://www.rossbencina.com/code/real-time-audio-programming-101-time-waits-for-nothing) rossbencina.com
- [bgfx](https://bkaradzic.github.io/bgfx/overview.html) bkaradzic.github.io
- [Box2D v3.2](https://github.com/erincatto/box2d/blob/main/docs/release_notes_v320.md) github.com
- [Bullet](https://github.com/bulletphysics/bullet3) github.com
- [Catto's GDC talks](https://box2d.org/publications/) box2d.org
- [CDLOD](https://github.com/fstrugar/CDLOD/blob/master/cdlod_paper_latest.pdf) github.com
- [Clavet, GDC 2016](https://gdcvault.com/play/1023280/Motion-Matching-and-The-Road) gdcvault.com
- [Courrèges](https://www.adriancourreges.com/blog/2016/09/09/doom-2016-graphics-study/) adriancourreges.com
- [dav1d](https://github.com/videolan/dav1d) github.com
- [Digital Production](https://digitalproduction.com/2025/09/02/blender-5-0-ships-built-in-aces-2-0-view-transform/) digitalproduction.com
- [DirectX blog](https://devblogs.microsoft.com/directx/directx-adopting-spir-v/) devblogs.microsoft.com
- [Doumler](https://timur.audio/using-locks-in-real-time-audio-processing-safely) timur.audio
- [Epic](https://dev.epicgames.com/documentation/unreal-engine/API/Runtime/RHI/FDynamicRHI) dev.epicgames.com
- [Epic Niagara](https://dev.epicgames.com/documentation/en-us/unreal-engine/overview-of-niagara-effects-for-unreal-engine) dev.epicgames.com
- [Epic RDG](https://dev.epicgames.com/documentation/en-us/unreal-engine/render-dependency-graph-in-unreal-engine) dev.epicgames.com
- [Ericson](https://realtimecollisiondetection.net/books/rtcd/) realtimecollisiondetection.net
- [FFmpeg download](https://ffmpeg.org/download.html) ffmpeg.org
- [FFmpeg legal](https://ffmpeg.org/legal.html) ffmpeg.org
- [Filament](https://github.com/google/filament) github.com
- [FrameGraph](https://www.slideshare.net/DICEStudio/framegraph-extensible-rendering-architecture-in-frostbite) slideshare.net
- [Frechette](https://nfrechette.github.io/2023/09/17/acl_in_ue) nfrechette.github.io
- [GamingOnLinux](https://www.gamingonlinux.com/2025/08/amd-fidelityfx-super-resolution-4-fsr-4-now-available-for-all-developers-with-fidelityfx-sdk-2-0) gamingonlinux.com
- [glTF spec](https://github.com/KhronosGroup/glTF/blob/main/specification/2.0/Specification.adoc) github.com
- [Godot AnimationTree](https://github.com/godotengine/godot-docs/blob/master/tutorials/animation/animation_tree.rst) github.com
- [Godot docs](https://github.com/godotengine/godot-docs/blob/master/tutorials/physics/using_jolt_physics.rst) github.com
- [GPUOpen](https://gpuopen.com/fidelityfx-cas/) gpuopen.com
- [Hillaire 2020](https://diglib7.eg.org/bitstream/handle/10.1111/cgf14050/v39i4pp013-022.pdf) diglib7.eg.org
- [JangaFX](https://jangafx.com/software/pricing) jangafx.com
- [Jolt README](https://github.com/jrouwe/JoltPhysics) github.com
- [Kavan et al. 2008](https://users.cs.utah.edu/~ladislav/kavan08geometric/kavan08geometric.html) users.cs.utah.edu
- [Khronos](https://www.khronos.org/news/press/khronos-releases-opengl-4.6-with-spir-v-support) khronos.org
- [Khronos](https://www.khronos.org/news/press/khronos-group-launches-slang-initiative-hosting-open-source-compiler-contributed-by-nvidia) khronos.org
- [Khronos gl.xml](https://github.com/KhronosGroup/OpenGL-Registry/blob/main/xml/gl.xml) github.com
- [Khronos ToneMapping](https://github.com/KhronosGroup/ToneMapping) github.com
- [LearnOpenGL](https://learnopengl.com) learnopengl.com
- [lightmapper](https://github.com/ands/lightmapper) github.com
- [Majercik et al. 2019](https://jcgt.org/published/0008/02/01/paper-lowres.pdf) jcgt.org
- [McGuire & Bavoil](https://jcgt.org/published/0002/02/09/paper.pdf) jcgt.org
- [miniaudio](https://github.com/mackron/miniaudio) github.com
- [miniaudio manual](https://miniaud.io/docs/manual/index.html) miniaud.io
- [NV_mesh_shader](https://github.com/KhronosGroup/OpenGL-Registry/blob/main/extensions/NV/NV_mesh_shader.txt) github.com
- [NVRHI](https://github.com/NVIDIAGameWorks/nvrhi/blob/main/doc/ProgrammingGuide.md) github.com
- [Olsson et al.](https://www.cse.chalmers.se/~uffe/clustered_shading_preprint.pdf) cse.chalmers.se
- [OpenAL Soft](https://github.com/kcat/openal-soft) github.com
- [opus-codec.org](https://opus-codec.org/) opus-codec.org
- [ozz features](https://guillaumeblanc.github.io/ozz-animation/documentation/features/) guillaumeblanc.github.io
- [Phoronix](https://www.phoronix.com/news/Theora-1.2-Released) phoronix.com
- [PhysX CCT](https://nvidia-omniverse.github.io/PhysX/physx/5.6.1/docs/CharacterControllers.html) nvidia-omniverse.github.io
- [PhysX CHANGELOG](https://github.com/NVIDIA-Omniverse/PhysX/blob/main/physx/CHANGELOG.md) github.com
- [pl_mpeg](https://github.com/phoboslab/pl_mpeg) github.com
- [RenderDoc](https://renderdoc.org/docs/getting_started/features.html) renderdoc.org
- [Routledge](https://www.routledge.com/Game-Audio-Programming-5-Principles-and-Practices/Somberg/p/book/9781032852850) routledge.com
- [Rouwe GDC 2022 notes](https://jrouwe.nl/architectingjolt/ArchitectingJoltPhysics_Rouwe_Jorrit_Notes.pdf) jrouwe.nl
- [RTXGI-DDGI](https://github.com/NVIDIAGameWorks/RTXGI-DDGI) github.com
- [SDL hint](https://wiki.libsdl.org/SDL3/SDL_HINT_AUDIO_DEVICE_SAMPLE_FRAMES) wiki.libsdl.org
- [SDL3 CategoryAudio](https://wiki.libsdl.org/SDL3/CategoryAudio) wiki.libsdl.org
- [sebh](https://github.com/sebh/UnrealEngineSkyAtmosphere) github.com
- [SMAA LICENSE](https://github.com/iryoku/smaa/blob/master/LICENSE.txt) github.com
- [Sousa & Geffroy](https://advances.realtimerendering.com/s2016/Siggraph2016_idTech6.pdf) advances.realtimerendering.com
- [Steam Audio](https://github.com/ValveSoftware/steam-audio) github.com
- [Steam Deck recommendations](https://partner.steamgames.com/doc/steamdeck/recommendations) partner.steamgames.com
- [Taylor & Francis](https://www.taylorfrancis.com/books/9781315157542/chapters/10.1201/b22247-9) taylorfrancis.com
- [The Forge](https://github.com/ConfettiFX/The-Forge) github.com
- [UE](https://docs.unrealengine.com/4.27/en-US/BlueprintAPI/AI/Perception/ReportNoiseEvent/index.html) docs.unrealengine.com
- [Unity VFX Graph](https://docs.unity3d.com/Packages/com.unity.visualeffectgraph@17.0/manual/System-Requirements.html) docs.unity3d.com
- [Unreal state machines](https://dev.epicgames.com/documentation/en-us/unreal-engine/state-machines-in-unreal-engine) dev.epicgames.com
- [vk.xml](https://github.com/KhronosGroup/Vulkan-Docs/blob/main/xml/vk.xml) github.com
- [vkguide](https://vkguide.dev/docs/gpudriven/compute_culling/) vkguide.dev
- [vkguide.dev](https://vkguide.dev) vkguide.dev
- [Vulkan-Guide](https://github.com/KhronosGroup/Vulkan-Guide) github.com
- [Wicked Engine](https://wickedengine.net/2017/11/gpu-based-particle-simulation/) wickedengine.net
- [Wikipedia](https://en.wikipedia.org/wiki/FMOD) en.wikipedia.org
- [Wikipedia](https://en.wikipedia.org/wiki/Bink_Video) en.wikipedia.org
- [Wronski](https://bartwronski.com/wp-content/uploads/2014/08/bwronski_volumetric_fog_siggraph2014.pdf) bartwronski.com
- [XAG 104](https://learn.microsoft.com/en-us/gaming/accessibility/xbox-accessibility-guidelines/104) learn.microsoft.com
- [xatlas](https://github.com/jpcy/xatlas) github.com
- [XeGTAO](https://github.com/GameTechDev/XeGTAO) github.com

### Gameplay 55

- [.NET docs](https://github.com/dotnet/docs/blob/main/docs/core/tutorials/netcore-hosting.md) github.com
- [AngelCode](https://www.angelcode.com/angelscript/) angelcode.com
- [Booth 2009](https://steamcdn-a.akamaihd.net/apps/valve/2009/ai_systems_of_l4d_mike_booth.pdf) steamcdn-a.akamaihd.net
- [BT.CPP](https://github.com/BehaviorTree/BehaviorTree.CPP/blob/HEAD/CHANGELOG.rst) github.com
- [Caini](https://skypjack.github.io/2019-02-14-ecs-baf-part-1/) skypjack.github.io
- [Champandard & Dunstan](http://www.gameaipro.com/GameAIPro/GameAIPro_Chapter06_The_Behavior_Tree_Starter_Kit.pdf) gameaipro.com
- [Cinemachine](https://docs.unity3d.com/Packages/com.unity.cinemachine@3.1/manual/concept-camera-control-transitions.html) docs.unity3d.com
- [Command](https://gameprogrammingpatterns.com/command.html) gameprogrammingpatterns.com
- [cr](https://github.com/fungos/cr) github.com
- [Deoccluder](https://docs.unity3d.com/Packages/com.unity.cinemachine@3.1/manual/CinemachineDeoccluder.html) docs.unity3d.com
- [Dirty Flag](https://gameprogrammingpatterns.com/dirty-flag.html) gameprogrammingpatterns.com
- [docs.rs](https://docs.rs/bevy/latest/bevy/ecs/message/index.html) docs.rs
- [DOD book](https://www.dataorienteddesign.com/dodbook/node2.html) dataorienteddesign.com
- [dotnet/core](https://github.com/dotnet/core/blob/main/release-notes/releases-index.json) github.com
- [ECS FAQ](https://github.com/SanderMertens/ecs-faq) github.com
- [EnTT](https://github.com/skypjack/entt/blob/HEAD/docs/md/entity.md) github.com
- [EnTT releases](https://github.com/skypjack/entt/releases) github.com
- [Event Queue](https://gameprogrammingpatterns.com/event-queue.html) gameprogrammingpatterns.com
- [flecs](https://github.com/SanderMertens/flecs/blob/master/docs/HierarchiesManual.md) github.com
- [flecs Prefabs](https://github.com/SanderMertens/flecs/blob/master/docs/PrefabsManual.md) github.com
- [Game Developer](https://gamedeveloper.com/design/video-50-common-game-camera-mistakes----and-how-to-fix-them) gamedeveloper.com
- [Game Developer](https://www.gamedeveloper.com/design/the-perfect-organism-the-ai-of-alien-isolation) gamedeveloper.com
- [GDC Vault](https://gdcvault.com/play/1023146/Math-for-Game-Programmers-Juicing) gdcvault.com
- [Godot](https://docs.godotengine.org/en/stable/getting_started/step_by_step/instancing.html) docs.godotengine.org
- [Graham](http://www.gameaipro.com/GameAIPro/GameAIPro_Chapter09_An_Introduction_to_Utility_Theory.pdf) gameaipro.com
- [Groot2](https://www.behaviortree.dev/groot) behaviortree.dev
- [Impulse](https://docs.unity3d.com/Packages/com.unity.cinemachine@3.1/manual/CinemachineImpulse.html) docs.unity3d.com
- [Isla 2005](https://gamedeveloper.com/programming/gdc-2005-proceeding-handling-complexity-in-the-i-halo-2-i-ai) gamedeveloper.com
- [isocpp.org](https://isocpp.org/blog/2015/01/cppcon-2014-data-oriented-design-and-c-mike-acton) isocpp.org
- [Kazemi](https://tinysubversions.com/spelunkyGen/) tinysubversions.com
- [Live++ pricing](https://liveplusplus.tech/pricing.html) liveplusplus.tech
- [lua.org](https://www.lua.org/versions.html) lua.org
- [LuaJIT](https://github.com/LuaJIT/LuaJIT/blob/v2.1/README) github.com
- [Luau](https://github.com/luau-lang/luau) github.com
- [luau.org](https://luau.org/sandbox) luau.org
- [MCV/DEVELOP](https://mcvuk.com/business-news/guest-post-integrating-ecs-into-existing-mass-market-games-a-practical-engineering-guide) mcvuk.com
- [Mertens](https://ajmmertens.medium.com/building-an-ecs-2-archetypes-and-vectorization-fe21690805f9) ajmmertens.medium.com
- [mono](https://github.com/mono/mono) github.com
- [Observer](https://gameprogrammingpatterns.com/observer.html) gameprogrammingpatterns.com
- [Orkin 2006](https://alumni.media.mit.edu/~jorkin/gdc2006_orkin_jeff_fear.pdf) alumni.media.mit.edu
- [Recast](https://github.com/recastnavigation/recastnavigation) github.com
- [Recast docs](https://github.com/recastnavigation/recastnavigation/blob/HEAD/Docs/_99_Roadmap.md) github.com
- [Recast Introduction](https://github.com/recastnavigation/recastnavigation/blob/HEAD/Docs/_1_Introduction.md) github.com
- [Red Blob Games](https://www.redblobgames.com/pathfinding/a-star/introduction.html) redblobgames.com
- [sol2](https://github.com/ThePhD/sol2) github.com
- [Spatial Partition](https://gameprogrammingpatterns.com/spatial-partition.html) gameprogrammingpatterns.com
- [UE 5.8](https://dev.epicgames.com/documentation/en-us/unreal-engine/world-partition-in-unreal-engine) dev.epicgames.com
- [UE 5.8 AI Perception](https://dev.epicgames.com/documentation/en-us/unreal-engine/ai-perception-in-unreal-engine) dev.epicgames.com
- [UE 5.8 BT](https://dev.epicgames.com/documentation/en-us/unreal-engine/behavior-tree-in-unreal-engine---overview) dev.epicgames.com
- [UE Gameplay Debugger](https://dev.epicgames.com/documentation/en-us/unreal-engine/using-the-gameplay-debugger-in-unreal-engine) dev.epicgames.com
- [Unity Entities](https://docs.unity3d.com/Packages/com.unity.entities@1.3/manual/concepts-archetypes.html) docs.unity3d.com
- [version.h](https://github.com/skypjack/entt/blob/HEAD/src/entt/config/version.h) github.com
- [Wasmtime](https://github.com/bytecodealliance/wasmtime/blob/main/docs/examples-interrupting-wasm.md) github.com
- [WaveFunctionCollapse](https://github.com/mxgmn/WaveFunctionCollapse) github.com
- [Wren](https://github.com/wren-lang/wren/blob/main/CHANGELOG.md) github.com

### Interface 30

- [AccessKit](https://github.com/AccessKit/accesskit) github.com
- [Clay](https://github.com/nicbarker/clay) github.com
- [CLDR](https://www.unicode.org/cldr/charts/latest/supplemental/language_plural_rules.html) unicode.org
- [Dear ImGui FAQ](https://github.com/ocornut/imgui/blob/master/docs/FAQ.md) github.com
- [FreeType license](https://freetype.org/license.html) freetype.org
- [FriBidi](https://github.com/fribidi/fribidi) github.com
- [GAG](https://gameaccessibilityguidelines.com/full-list/) gameaccessibilityguidelines.com
- [HarfBuzz](https://github.com/harfbuzz/harfbuzz) github.com
- [ICU downloads](https://unicode-org.github.io/icu/download) unicode-org.github.io
- [Microsoft](https://learn.microsoft.com/en-us/globalization/methodology/pseudolocalization) learn.microsoft.com
- [msdf-atlas-gen](https://github.com/Chlumsky/msdf-atlas-gen) github.com
- [Noesis](https://www.noesisengine.com/licensing.php) noesisengine.com
- [Project Fluent](https://projectfluent.org/) projectfluent.org
- [RmlUi](https://github.com/mikke89/RmlUi) github.com
- [RmlUi releases](https://github.com/mikke89/RmlUi/releases) github.com
- [SDL3 BestKeyboardPractices](https://wiki.libsdl.org/SDL3/BestKeyboardPractices) wiki.libsdl.org
- [SDL3 CategoryGamepad](https://wiki.libsdl.org/SDL3/CategoryGamepad) wiki.libsdl.org
- [SDL3_ttf](https://wiki.libsdl.org/SDL3_ttf/CategorySDLTTF) wiki.libsdl.org
- [SDL_CommonEvent](https://wiki.libsdl.org/SDL3/SDL_CommonEvent) wiki.libsdl.org
- [SDL_GameControllerDB](https://github.com/mdqinc/SDL_GameControllerDB) github.com
- [SDL_StartTextInput](https://wiki.libsdl.org/SDL3/SDL_StartTextInput) wiki.libsdl.org
- [SDL_ttf](https://github.com/libsdl-org/SDL_ttf) github.com
- [SheenBidi](https://github.com/mta452/SheenBidi) github.com
- [Steam Deck compatibility](https://partner.steamgames.com/doc/steamdeck/compat) partner.steamgames.com
- [Steam Languages](https://partner.steamgames.com/doc/store/localization/languages) partner.steamgames.com
- [Ultralight](https://ultralig.ht/pricing/) ultralig.ht
- [Unreal Enhanced Input](https://dev.epicgames.com/documentation/en-us/unreal-engine/enhanced-input-in-unreal-engine) dev.epicgames.com
- [XAG](https://learn.microsoft.com/en-us/gaming/accessibility/guidelines) learn.microsoft.com
- [XAG 118](https://learn.microsoft.com/en-us/gaming/accessibility/xbox-accessibility-guidelines/118) learn.microsoft.com
- [Yoga](https://github.com/facebook/yoga) github.com

### Tools 43

- [Backtrace pricing](https://backtrace.io/pricing) backtrace.io
- [Box2D](https://box2d.org/posts/2024/08/determinism/) box2d.org
- [Catch2](https://github.com/catchorg/Catch2) github.com
- [ccache](https://github.com/ccache/ccache) github.com
- [CMake](https://github.com/Kitware/CMake/releases/tag/v4.4.5) github.com
- [cmake-presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html) cmake.org
- [Crashpad](https://chromium.googlesource.com/crashpad/crashpad/+/HEAD/doc/overview_design.md) chromium.googlesource.com
- [Dear ImGui](https://github.com/ocornut/imgui) github.com
- [debuginfod](https://sourceware.org/elfutils/Debuginfod.html) sourceware.org
- [doctest](https://github.com/doctest/doctest) github.com
- [EDPB 2/2023](https://www.edpb.europa.eu/system/files/documents/2024-10/edpb_guidelines_202302_technical_scope_art_53_eprivacydirective_v2_en_0.pdf) edpb.europa.eu
- [efsw](https://github.com/SpartanJ/efsw) github.com
- [European Commission](https://commission.europa.eu/law/law-topic/data-protection/data-protection-explained_en) commission.europa.eu
- [GameAnalytics](https://gameanalytics.com/pricing/) gameanalytics.com
- [git-lfs-lock](https://github.com/git-lfs/git-lfs/blob/main/docs/man/git-lfs-lock.adoc) github.com
- [GitHub](https://github.com/resources/insights/2026-pricing-changes-for-github-actions) github.com
- [GitHub Docs](https://docs.github.com/en/billing/concepts/product-billing/git-lfs) docs.github.com
- [Godot](https://docs.godotengine.org/uk/4.4/_sources/contributing/development/core_and_modules/unit_testing.rst.txt) docs.godotengine.org
- [GoogleTest](https://github.com/google/googletest) github.com
- [im3d](https://github.com/john-chapman/im3d) github.com
- [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo) github.com
- [license](https://github.com/ocornut/imgui_test_engine/blob/main/imgui_test_engine/LICENSE.txt) github.com
- [Live++ docs](https://liveplusplus.tech/docs/documentation.html) liveplusplus.tech
- [Microsoft Learn](https://learn.microsoft.com/en-us/windows/win32/api/minidumpapiset/nf-minidumpapiset-minidumpwritedump) learn.microsoft.com
- [Perforce](https://www.perforce.com/products/helix-core/free-version-control) perforce.com
- [RenderDoc](https://github.com/baldurk/renderdoc/releases/tag/v1.47) github.com
- [RenderDoc API](https://renderdoc.org/docs/in_application_api.html) renderdoc.org
- [sccache](https://github.com/mozilla/sccache) github.com
- [Sentry docs](https://docs.sentry.io/platforms/native/) docs.sentry.io
- [Sentry pricing](https://sentry.io/pricing/) sentry.io
- [sentry-native](https://github.com/getsentry/sentry-native) github.com
- [sentry.h](https://github.com/getsentry/sentry-native/blob/master/include/sentry.h) github.com
- [Steamworks](https://partner.steamgames.com/doc/features/error_reporting) partner.steamgames.com
- [SymStore](https://learn.microsoft.com/en-us/windows/win32/debug/using-symstore) learn.microsoft.com
- [systemd-coredump](https://www.freedesktop.org/software/systemd/man/latest/systemd-coredump.html) freedesktop.org
- [Tracy](https://github.com/wolfpld/tracy) github.com
- [trunkbaseddevelopment.com](https://trunkbaseddevelopment.com/) trunkbaseddevelopment.com
- [Unity](https://support.unity.com/hc/en-us/articles/34748492914964-Understanding-Unity-DevOps-charges) support.unity.com
- [Unreal](https://dev.epicgames.com/documentation/en-us/unreal-engine/screenshot-comparison-tool-in-unreal-engine) dev.epicgames.com
- [Unreal asserts](https://dev.epicgames.com/documentation/en-us/unreal-engine/asserts-in-unreal-engine) dev.epicgames.com
- [Unreal logging](https://dev.epicgames.com/documentation/en-us/unreal-engine/logging-in-unreal-engine) dev.epicgames.com
- [Unreal OFPA](https://dev.epicgames.com/documentation/en-us/unreal-engine/one-file-per-actor-in-unreal-engine) dev.epicgames.com
- [vcpkg](https://learn.microsoft.com/en-us/vcpkg/consume/binary-caching-overview) learn.microsoft.com

### Online 43

- [ASF](https://apache.googlesource.com/www-site/+/HEAD/content/legal/apply-license.md) apache.googlesource.com
- [Bernier](https://developer.valvesoftware.com/wiki/Latency_Compensating_Methods_in_Client/Server_In-game_Protocol_Design_and_Optimization) developer.valvesoftware.com
- [comss.ru](https://www.comss.ru/page.php?id=21392) comss.ru
- [Docker](https://docs.docker.com/engine/network/port-publishing/) docs.docker.com
- [docs](https://spacetimedb.com/docs) spacetimedb.com
- [EA](https://forums.ea.com/blog/apex-legends-game-info-hub-en/dev-team-update-linux--anti-cheat/9468053) forums.ea.com
- [EOS](https://onlineservices.epicgames.com/llms.txt) onlineservices.epicgames.com
- [EOS news](https://onlineservices.epicgames.com/news/epic-online-services-launches-anti-cheat-support-for-linux-mac-and-steam-deck) onlineservices.epicgames.com
- [European Commission](https://commission.europa.eu/law/law-topic/data-protection/information-business-and-organisations/dealing-individuals-requests_mt) commission.europa.eu
- [EVE](https://www.eveonline.com/article/time-dilation-hows-that-going) eveonline.com
- [Fiedler](https://gafferongames.com/post/deterministic_lockstep/) gafferongames.com
- [FTC](https://www.ftc.gov/node/60359) ftc.gov
- [GNS](https://github.com/ValveSoftware/GameNetworkingSockets) github.com
- [Godot](https://raw.githubusercontent.com/godotengine/godot-docs/master/tutorials/export/exporting_for_dedicated_servers.rst) raw.githubusercontent.com
- [Godot steam_tracker.cpp](https://cocalc.com/github/godotengine/godot/blob/master/main/steam_tracker.cpp) cocalc.com
- [GOG](https://docs.gog.com/sdk/) docs.gog.com
- [LICENSE](https://raw.githubusercontent.com/clockworklabs/SpacetimeDB/master/LICENSE.txt) raw.githubusercontent.com
- [Microsoft Game Dev](https://developer.microsoft.com/games/publish) developer.microsoft.com
- [Nintendo Developer Portal](https://developer.nintendo.com/register) developer.nintendo.com
- [NME](https://www.nme.com/news/valve-says-steam-deck-devs-can-contact-battleye-to-enable-anti-cheat-3089079) nme.com
- [Replication Graph](https://dev.epicgames.com/documentation/en-us/unreal-engine/replication-graph-in-unreal-engine) dev.epicgames.com
- [Riot](https://technology.riotgames.com/node/112) technology.riotgames.com
- [SIE](https://sonyinteractive.com/en/?p=3002341) sonyinteractive.com
- [SpacetimeDB](https://github.com/clockworklabs/SpacetimeDB) github.com
- [State Synchronization](https://gafferongames.com/post/state_synchronization/) gafferongames.com
- [Steam Direct](https://partner.steamgames.com/steamdirect) partner.steamgames.com
- [Steam DLC](https://partner.steamgames.com/doc/store/application/dlc) partner.steamgames.com
- [Steam DRM](https://partner.steamgames.com/doc/features/drm) partner.steamgames.com
- [Steam Leaderboards](https://partner.steamgames.com/doc/features/leaderboards) partner.steamgames.com
- [Steam Matchmaking](https://partner.steamgames.com/doc/features/multiplayer/matchmaking) partner.steamgames.com
- [Steam Overlay](https://partner.steamgames.com/doc/features/overlay) partner.steamgames.com
- [Steam Workshop](https://partner.steamgames.com/doc/features/workshop) partner.steamgames.com
- [Steamworks](https://partner.steamgames.com/doc/gettingstarted/contentsurvey/germany) partner.steamgames.com
- [Steamworks API](https://partner.steamgames.com/doc/sdk/api) partner.steamgames.com
- [Steamworks SDR](https://partner.steamgames.com/doc/features/multiplayer/steamdatagramrelay) partner.steamgames.com
- [systemd.service](https://www.freedesktop.org/software/systemd/man/latest/systemd.service.html) freedesktop.org
- [tc-netem](https://man7.org/linux/man-pages/man8/tc-netem.8.html) man7.org
- [UGS pricing](https://unity.com/products/gaming-services/pricing) unity.com
- [Unity](https://docs.unity3d.com/Packages/com.unity.netcode@1.6/manual/optimizations.html) docs.unity3d.com
- [Unreal](https://dev.epicgames.com/documentation/en-us/unreal-engine/networking-overview-for-unreal-engine) dev.epicgames.com
- [VAC](https://partner.steamgames.com/doc/features/anticheat/vac_integration?l=romanian) partner.steamgames.com
- [Valve SMN mirror](https://gist.github.com/CoolOppo/fe0586836de3fb2f90f9) gist.github.com
- [XR-001](https://devdocs.xbox.com/publishing/certification/xr/xr-001.md) devdocs.xbox.com

### Added in this review 15

- [Game Engine Architecture, 4th edition, two-volume set (CRC Press)](https://www.routledge.com/Game-Engine-Architecture-Two-Volume-Set/Gregory/p/book/9781041162599) routledge.com
- [ICU project site, release news](https://icu.unicode.org/) icu.unicode.org
- [FMOD licensing page](https://fmod.com/licensing) fmod.com
- [iTechGuides, FMOD Studio pricing (checked 20 Sep 2026; secondary)](https://www.itechguides.com/products/fmod-studio/pricing/) itechguides.com
- [Bloomberg clang-p2996 fork](https://github.com/bloomberg/clang-p2996) github.com
- [Whole Tomato, C++26 reflection for large codebases (secondary)](https://www.wholetomato.com/blog/cpp26-reflection-large-codebases) wholetomato.com
- [NVIDIA, PhysX 5 open-source release post](https://developer.nvidia.com/blog/open-source-simulation-expands-with-nvidia-physx-5-release) developer.nvidia.com
- [Unity manual, Time and frame rate management](https://docs.unity3d.com/2023.2/Documentation/Manual/TimeFrameManagement.html) docs.unity3d.com
- [Epic, Forward Shading Renderer in Unreal Engine](https://dev.epicgames.com/documentation/en-us/unreal-engine/forward-shading-renderer-in-unreal-engine) dev.epicgames.com
- [Epic, Generating a State Object Database for Advanced Shader Delivery](https://dev.epicgames.com/documentation/en-us/unreal-engine/generating-a-state-object-database-for-advanced-shader-delivery) dev.epicgames.com
- [Valve, Steam Deck FAQ](https://partner.steamgames.com/doc/steamdeck/faq) partner.steamgames.com
- [SpacetimeDB.Runtime 2.8.0 license text](https://feed.nuget.org/packages/SpacetimeDB.Runtime/2.8.0/License) feed.nuget.org
- [Fenwick, CVAA compliance waiver for video games (secondary, legal)](https://www.fenwick.com/insights/publications/cvaa-compliance-waiver-for-video-games-set-to-expire-in-december-2018) fenwick.com
- [Frankfurt Kurnit, Video games with advanced communications services (secondary, legal)](https://fkks.com/news/video-games-with-advanced-communications-services-must-now-be-accessible-to-players-with-disabilities) fkks.com
- [FMOD Studio product page, pricing tiers](https://fmod.com/studio) fmod.com

Restructured from the original report “Game engine roadmap, all subsystems”. Versions, prices and licenses are as observed in early November 2026. Legal notes here are not legal advice.

[Map](#category-map)