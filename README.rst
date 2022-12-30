Dr. Mario
=========

Use cases
---------

1. Single player streamed with OBS.

   In OBS, add the Dr. Mario filter to the game video capture.

2. Tournament match streamed with OBS.

   Players streams without Dr. Mario filter. The restreamer creates a
   two player tournament scene in OBS and adds the Dr. Mario filter on
   the scene (a group). Not possible with async filter unfortunately.

Statistics to show
------------------

- Viruses cleared per pill (moving average).

- Pills per second (moving average).

- Combos: Fat log, sweet-t, lightning, ...

- Number of singles, doubles and triples.

- Pill counts. Total and per color.

- Viruses left per color.

- Clears with no viruses.

- Unusable pill burst.

Output from image analysis
--------------------------

- New pill and its colors.

- Pills, viruses and clear locations in the bottle.

- Player clocks?

- Standings?
