# NFSU/NFSU2 Visual Reputation Remover

Need for Speed Underground 1 & 2 have an inconvenient system: Visual Reputation (also known as Star Reputation and Visual Rating).

This system rates the player's car based on which Visual Upgrades are installed in it. I do not think this is a good system. Visual customization should be an expression of the player's taste, not a quantitative metric based on amount of parts and how expensive they are. The problem is worsened by the fact this system interacts with other game systems, making it difficult or impossible to progress through the game in case the player doesn't want to rice his car to extreme levels.

This patch replaces the visual reputation system by one that respects player choice, allowing game progress and rewards regardless of the visual appearance of the player's car.

## Game Changes

### Underground 1

The following changes are applied to NFSU:

 + Star Rating is based on how many parts are _unlocked_ instead of installed.
 + Star Rating is gradually increased until the first part is unlocked in race 18.
 + Style Points Multiplier interact with the system described above.
 + Races 14 and 30 are not blocked by the car's customization level since they interact with the patched system.
 + Magazines 15, 16, 17, 18 and 19 can be unlocked by either customizing the car (original system) or by completing races 20, 30, 80, 105 and 112 respectively.

### Underground 2

The Visual Rating system in Underground 2 only interacts with the availability of DVD Covers. Therefore this patch fixes the rating at values good enough to acquire the covers for each game stage, as such, you receive SMS about cover opportunities at the beginning of each stage.

 + Stage 0, 1, 2, 3 and 4 are fixed with 0, 1, 3, 5½ and 8 stars respectively.
 + Stage 5 onwards are fixed with 10 stars.
 + Visual Rating Meter is removed from the customization menu.

## Building

To compile `novisualrep.asi` yourself, you need [premake5](https://premake.github.io/). Execute the following at the root of this repository to generate a project for your preferred toolchain:

     premake5 vs2013
     # Change vs2013 to the toolchain of your choice. See `premake5 --help`.

Check the [releases page](https://github.com/thelink2012/novisualrep/releases) for pre-compiled binaries.

## Installing

  1. Extract [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate_ASI_Loader/releases) into your `<path-to-game>` directory. If you have ThirteenAG's & Aero's Widescreen Fix, this step is unnecessary.
  2. Copy `novisualrep.asi` into `<path-to-game>/scripts`.
  3. Have fun.

