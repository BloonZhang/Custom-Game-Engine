# Custom C++ Game Engine

The Custom C++ Game Engine was a coursework project spanning an entire semester. The goal was to create an engine that uses JSON as a scripting language, similar to how Unreal Engine uses blueprints or how Unity uses C#. Below are the breakdowns of the individual components in this engine.

You can also see more here:\
https://zhangljeffrey.wixstudio.com/portfolio/projects/cpp-game-engine
<br/><br/>
## Components
### Datum
Datum is a custom data structre whose type is determined at runtime instead of compile-time. This allows for a way to store data whose type is not known until runtime.

In simple terms, Datum is a data structure similar to std::vector, but it can dynamically transform between vector\<int>, vector\<string>, etc.
<br/><br/>
### Scope
Scope is a class that holds multiple Datums. In concept, its job is simple, but in practice it allows for Datum to exist in a hierarchical structure. Because a single Scope can hold multiple Datums, and because a Datum can hold any data type including Scope, I can create a nested arrangement of Scopes and Datums. 

An analogy would be the GameObject hierarchy in Unity. Because GameObjects can hold other GameObjects, the user has the ability to create deep parent-child structures. Similarly, Scope and Datum exist to allow the game engine to create deep parent-child structures.
<br/><br/>
### Attributed
Attributed serves as the bridge between C++ data and the Datum/Scope tree. Attributed is a base class that other classes can derive from. As part of the constructor, any class inheriting from Attributed will automatically create a Scope/Datum tree that represents its C++ data members.

For example, if I had a class Player that had a integer variable Health, and Player inherits from Attributed, then when a Player is constructed, it will also construct a corresponding Datum\<int> that represents Health.
<br/><br/>
### GameObject
GameObjects represent the "nouns" of the game engine. They have a name, a transform, and the capability to hold children GameObjects. Of course, the hierarchy of GameObjects is represented by the Scope/Datum hierarchy. They also have an update function, similar to MonoBehaviour's Update() in Unity.
<br/><br/>
### Action
Actions represent the "verbs" of the game engine. In the simplest terms, an Action takes in any number of Datums and "does something" to them.

A simple example would be a class called IncrementAction, which can take a Datum\<int> and increase its value by 1. A simple application would be to take a GameObject named Player, and specify that it should run IncrementAction on a Datum\<int> named Health. Any time this action is executed, it would be the equivalent of increasing the player's health by 1.
<br/><br/>
### Reaction
_Please note: Reaction.h and Reaction.cpp is not present in this repo. Due to the schedule of the curriculum, I developed Reaction as part of a group project, and thus my code is not compatible with my solo game engine. However, the code is saved in a separate private repo._

Reactions represent the "brains" of the game engine. Using a custom Event system, Reactions allow Actions to be executed as a callback.

A simple example would be definining a Reaction that subscribes a PlaySFXAction to a WeaponFiredEvent.
<br/><br/>
### Parse Coordinator
PraseCoordinator is the final key piece that brings all the components together into a game engine.

ParseCoordinator handles reading a JSON file and creating a corresponding Scope/Datum tree. Utilizing the Factory pattern, the JSON script is converted into GameObjects, Action, and Reactions. Therefore, a game can be created using JSON to define what GameObjects need to be created, and what Actions should be taken as a Reaction to gameplay events.
<br/><br/>
