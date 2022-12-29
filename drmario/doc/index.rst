About
=====

The Dr. Mario package in the `Mys programming language`_.

Input format
============

Nicely formatted for readability. The program only accepts single line
JSON documents.

.. code-block:: json

   {
       "type": "start",
       "timestamp": "199912389123.232"
   }

.. code-block:: json

   {
       "type": "stop",
       "timestamp": "199912389128.243"
   }

.. code-block:: json

   {
       "type": "game",
       "timestamp": "199912389125.144",
       "level": 13,
       "speed": "medium",
       "viruses": 23,
       "next_pill": ["red", "blue"],
       "bottle": [
           {
               "type": "pill",
               "row": 1,
               "column": 5,
               "color": "red"
           },
           {
               "type": "clear",
               "row": 1,
               "column": 6,
               "color": "blue"
           },
           {
               "type": "virus",
               "row": 3,
               "column": 8,
               "color": "yellow"
           }
       ]
   }

.. code-block:: json

   {
       "type": "game-delta",
       "timestamp": "199912389125.244",
       "bottle": [
           {
               "type": "pill",
               "row": 2,
               "column": 5,
               "color": "blue"
           }
       ]
   }

.. _Mys programming language: https://mys-lang.org
