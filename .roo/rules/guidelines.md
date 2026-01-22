# Project Guidelines (Game POC 2 - Isometric 2D Game with C++/Raylib)

**IMPORTANT**: Always refer to this file as the authoritative source for project context, architecture, and coding standards when working on prompts or modifications.

## Architecture (MVC Pattern)
1. **Model**: Entities (inherit from `Entity` which inherits from `Subject` for event dispatching)
2. **Controller**: Commands (handle game logic and input)
3. **View**: Presentation-only logic (sprites, animations, UI)

## Coding Standards
1. **Consistency**: Follow existing code style (e.g., indentation, naming conventions).
2. **Modularity**: Keep components reusable and decoupled.
3. **Documentation**: Comment complex logic and document public APIs.

## Project Structure
1. **Assets**: All game assets (images, sounds) go in `/resource` folder.
2. **Source**: Follow current folder structure:
   - `src/command/`: Command pattern implementations
   - `src/common/`: Shared utilities and base classes
   - `src/components/`: Component-based systems
   - `src/entity/`: Game entities and models
   - `src/network/`: ENET7 networking implementation
   - `src/scene/`: Game scenes and state management
   - `src/view/`: Presentation and rendering logic
3. **Network**: Use ENET7 for networking (see `src/network`).

## Workflow Rules
1. **When modifying code**:
   - Entities must inherit from `Entity` (which inherits `Subject`)
   - Commands handle game logic/input
   - Views handle presentation only
2. **Assets**: Always reference from `/resource`
3. **Network**: Use existing ENET7 wrapper

## Best Practices
1. **Performance**: Optimize draw calls and network packets.
2. **Event-Driven**: Use `Subject`/observer pattern for game events.
3. **Separation of Concerns**: Keep MVC layers distinct.