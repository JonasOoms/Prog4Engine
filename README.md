# Programming 4 Engine by Ooms Jonas

For programming 4 this semester, we had to build an engine from (relative) scratch. This project is based on the Minigin template, which was modified to form this end result. The engine aims to be relatively non-obstructive in the coding of your game. It provides bindings for controllers, basic services to make sure that users can define their own and register them, a component system to work in a modern, component driven way and Observer based event logic. 

# Minigin

Minigin is a very small project using [SDL2](https://www.libsdl.org/) and [glm](https://github.com/g-truc/glm) for 2D c++ game projects. It is in no way a game engine, only a barebone start project where everything sdl related has been set up. It contains glm for vector math, to aleviate the need to write custom vector and matrix classes.


# Input System
The input system in the engine works by providing each controller with a unique input mapping, which themselves have input bindings. Each player controller is lazy loaded: it's only created when it's needed. Player controllers each have unique input bindings, which provide a way for users to define their own mappings. These mappings query their individual bindings; the bindings hold commands which are to be triggered upon the desired button/key press. The input system was designed this way to provide a flexible but easy to use, RAII compliant way of assigning unique controls to different controllers. Users are in full control of the controllers they need, and the difficult parts are left up to the engine. The user simply has to define a new mapping, define some bindings, and then move their bindings into the controller. The engine and the user are in full understand of their respective ownership.

# Commands

The engine uses commands for user input logic and AI interaction. The input system uses commands in order to do work on the game environment, systems or whichever goal the user has in mind. Command provides an interface for easy to use commands, so that the user can define their own. By using commands, users seeking to make AI agents can use these to their advantage to try to more loosely couple their agents from the world.

# Service locator
The service locator is a way for users of the engine to pick and choose their implementation of certain engine features, in this case the Sound System and the Physics System. By ensuring ownership, the engine makes sure that the service resources are taken care of correctly when needed, while also allowing for flexibility in use and extension. Because of the service locator, users can implement and extend engine functionality. Services provide an interface for their methods, and it's up to the defined systems to implement these methods and ensure functionality.

# Sound Service
The sound service is a way for users to be able to play, register and mute sounds easily through the service locator. The engine provides a default implementation in SDL_SoundSystem, which implements an audio service that plays its audio through SDL_Mixer. It has its own audio thread that takes care of playing and loading audio, which is registered through the interface. Its possible to use the in built registers class to define a register which holds the sound_ids created by the sound service to map these ids to readable strings. SDL_SoundSystem has a built in request queue which parses the requests, keeping the implementation abstracted behind the play method, which is found in the SoundService interface.

# Physics Service 
The physics service provides the following: a way of querrying collisions and taking care of the movement by the use of PhysicsComponents, which hold the bounds and velocities of objects. The engine takes care of the registering and unregistering itself by providing the user with PhysicsComponents, which registers and unregisters itself automatically through the Physics Service. Users can and should extend PhysicsComponent functionality to provide an easy way of scripting collision responses. 
The engine provides an example of a physics system in the SimpleSpatialPhysicsSystem. This system takes care of collisions by querrying only the neighbors in a grid based spatial partition. It is to be noted that the SimpleSpatialPhysicsSystem might not be enough for every application, therefore the Service Locator provides a way of registering custom user systems, which includes the Physics Service. By using physics as a service, the engine guarrantees that the user has enough control over their own game so that they can create any 2D world.

# Game Objects and Component System
The engine provides a component system where game objects hold a basic transform, and a collection of components, which are themselves ways that the game loop can send messages through the game objects to the components so that they can correctly interact with the game world. The component class is an easy way for users to define their own logic to interact with the engine and their own game implementation. Game objects also provide a way for scene graphs by use of the parenting system. Game object transform take parents into account and make local space transforms possible.

By using game objects, we circumvent the problem of creation and deletion of objects in the world, by having the engine take care of object lifetime.

# Scene managing and traveling
The scene manager provides a way to select new scenes, which makes sure that players can traverse to other scenes, which also sends a message to game objects that a new scene is loaded, so that users can implement behaviour that triggers when new scenes are loaded. This way, users can have dynamic generation of scenes, log important data necessary for traversal or dynamic input bindings depending on the scene, for example.

# Observers
The engine uses observers as a way to pass messages around. Users can extend the observer class to listen for events coming from EventDispatchers. EventDispatchers send events to interested parties, who subscribe themselves to the dispatcher. These dispatchers send events through the Notify method, where observers parse the events and get the necessary information from it. The event system works with and Event Type, which is an id that denotes a type of event. These event types are important for the event context, which is implemented as a std::any. This makes sure that the event has a light weight way of passing arguments around and observers can std::any_cast the context to whichever one the event type requires. A hashing function is provided in the engine for users to make their own event types.  
