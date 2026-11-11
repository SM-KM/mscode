# Patterns

## Patterns

- [x] Builder pattern
  - [x] Classic builder + Director
  - [x] Fluent builder (CRTP, C++20)
  - [x] Fluent builder (deducing this, C++23)
  - [x] Type state builder
  - [x] Generic builder for any struct
  - [x] Validated build [c++23]
- [x] Factory pattern
  - [x] Simple factory
  - [x] Factory Method
  - [x] Registry factory
  - [x] Abstract Factory (static)
  - [x] Abstract Factory (dynamic)
  - [x] Generic Abstract Factory (type list)

### Creational
- [ ] Factory Method
- [ ] Abstract Factory
- [ ] Builder
- [ ] Prototype (`clone()`)
- [ ] Singleton (Meyers)
- [ ] Object Pool
- [ ] Registry / Service Locator

### Structural
- [ ] Adapter
- [ ] Bridge
- [ ] Composite
- [ ] Decorator
- [ ] Facade
- [ ] Flyweight
- [ ] Proxy
- [ ] Pimpl (opaque pointer)
- [ ] Type Erasure (`std::function`, `std::any`)

### Behavioral
- [ ] Strategy
- [ ] Observer / Signals-Slots
- [ ] Command
- [ ] State
- [ ] Template Method
- [ ] Visitor (`std::visit`)
- [ ] Iterator / Ranges
- [ ] Chain of Responsibility
- [ ] Mediator
- [ ] Memento
- [ ] Interpreter

### C++ idioms
- [ ] RAII
- [ ] CRTP
- [ ] Policy-Based Design
- [ ] Tag Dispatch
- [ ] SFINAE / concepts
- [ ] Copy-and-Swap
- [ ] Rule of 0/3/5
- [ ] Non-Virtual Interface
- [ ] Expression Templates
- [ ] Erase-Remove
- [ ] Passkey
- [ ] Scope Guard
- [ ] Deducing `this` (C++23)

### Concurrency
- [ ] Thread Pool
- [ ] Producer-Consumer
- [ ] Active Object
- [ ] Future/Promise
- [ ] Reactor
- [ ] Double-Checked Locking
- [ ] Read-Copy-Update
- [ ] Hazard Pointers

## Game programming patterns

- [ ] Command (input, undo/redo)
- [ ] Flyweight (shared tile/mesh data)
- [ ] Observer (achievements, events)
- [ ] Prototype (spawn from templates)
- [ ] Singleton (use sparingly)
- [ ] State (player/AI state machines)
- [ ] Game Loop (fixed timestep, variable render)
- [ ] Update Method
- [ ] Bytecode (scripting VM)
- [ ] Subclass Sandbox
- [ ] Type Object
- [ ] Component
- [ ] Event Queue
- [ ] Service Locator (audio, logging)
- [ ] Data Locality (AoS vs SoA)
- [ ] Dirty Flag
- [ ] Object Pool (bullets, particles)
- [ ] Spatial Partition (grid, quadtree, BVH)
- [ ] Entity Component System (archetypes, sparse sets)
- [ ] Behavior Trees / GOAP / Utility AI
- [ ] Scene Graph
- [ ] Double Buffer
- [ ] Fixed-Timestep Accumulator
- [ ] Resource Handle / Generational Index
- [ ] Command Buffer
- [ ] Job System / Task Graph
