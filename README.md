# UE5-Roguelike
![alt text](https://github.com/rzukale/UE5-Roguelike/blob/main/media/rl_ultimate.png?raw=true)
# Links
  - Gameplay video https://youtu.be/EsagvHmz1MA
  - Download and play packaged executable via [itch.io](https://antaran.itch.io/roguelike-project)


# Feature list
  - Multiplayer support (Steam enabled).
  - Third Person Character Movement.
  - Custom Gameplay Ability System (Actions):
    - Teleport.
    - Burning DoT-effect.
    - Damage reflect buff.
    - Blackhole ultimate.
    - Sprint ability.
    - Projectile attack.
  - Savegame system.
  - Using events to drive UI.
  - UI elements for player attributes and projected widgets for powerups and monsters.
  - GameplayTags to mark up Actors, Actions, Buffs.
  - Gamemode logic:
    - Datatable for holding monster information.
    - Data Assets for holding monster configurations.
    - Monster spawning system.
    - EQS for determining Monster & powerup spawn locations.
  - Asset Manager for Async loading data assets.
  - AI:
    - EQS for attack & finding cover
    - Behavior tree to handle roaming/chasing/attacking/fleeing/healing.
  - Powerup consumables to gain health/credits/abilities.
  - Main menu to host game and go to Join menu:
    - Join menu to list available sessions to choose from and join.

  # Compiling
  Install Unreal Engine 5.2  
  Install Visual Studio 2022, follow installation guide from [Epic](https://dev.epicgames.com/documentation/en-us/unreal-engine/setting-up-visual-studio-development-environment-for-cplusplus-projects-in-unreal-engine?application_version=5.2)  
  After cloning the project, right click RogueLike.uproject, if you want the .sln file, select "Generate Visual Studio project files". Or if you just want to launch the editor, select "Launch Game" and select "Yes" when prompted to build the project.
  
  # Usage
    - Built with UE 5.2 & Visual Studio 2022.
    - WASD-keys to move
    - Q: Blackhole ultimate (requires 50 Rage (You gain rage from taking damage. Progress bar in lower left in HUD))
    - E: Teleport
    - F: Interact with consumables
    - R: Parry-ability
    - Mouse left-button: fire projectiles
    - Left-Shift: Sprint ability (Gained via Powerup in the game-world)
    - Note on multiplayer:
      - Steam enabled so you will need 2 separate PCs to network (since you can only run 1 steam account per PC)
