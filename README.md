# NFSU2 - Visual Reputation Remover 

Need for Speed Underground 2 has an annoying feature: Visual Reputation.

This feature is a big incovenience on player's taste. You need to turn your beauty, minimalist vehicle into a carnival float to get into DVD covers and progress through the career.

This plugin removes the *Visual Reputation Meter* from the garage/tuning menu, so you can truly forget about it, and makes the *Visual Reputation* value increase as you go through the career stages:

  * Stage 0 is fixed with 0 stars of visual reputation;
  * Stage 1 is fixed with 1 star;
  * Stage 2 is fixed with 3 stars;
  * Stage 3 is fixed with 5 and a half stars;
  * Stage 4 is fixed with 8 stars;
  * Stage 5 onwards is fixed with 10 stars.

Now, have fun tuning your vehicles the way you want.

## Installing

  1. Extract [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate_ASI_Loader/releases) into your *Need For Speed Underground 2/* directory.
  2. Copy *novisualrep.asi* into the *scripts/* directory.
  3. Have fun.

You may also inject *novisualrep.asi* manually, after all it's just a native *.dll* with another extension.

## Compiling

If you wish to compile *novisualrep.asi* yourself, run the following to generate a project for your prefered toolchain:

     premake5 vs2013
     # Change vs2013 to the toolchain of your choice, see premake5 --help

