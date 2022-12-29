Dr. Mario
=========

Use cases
---------

1. Single player streamed with OBS.

   In OBS, add the Dr. Mario filter to the game video capture.

2. Tournament match streamed with OBS.

   Option 1: Players streams without Dr. Mario filter. The restreamer
             creates a two player tournament scene and adds the
             Dr. Mario filter on the scene (a group).

   Option 2: Players do not stream the game, but only sends JSON
             documents describing the game to the "restreamer".

Statistics to show
------------------

- Pill counts. Total and per color.

- Viruses cleared per pill.

- Combo count. Fat log, sweet-t, lightning.

- Viruses left per color.

- Lines cleared with no viruses.

- Pills per second.

- Unusable pill burst.

Output from image analysis
--------------------------

- New pill and its colors.

- Pills, viruses and clear locations in the bottle.
