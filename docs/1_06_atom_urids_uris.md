# 06 - Atom, URIDs & URIs

See video: <https://youtu.be/EHpwO5yl66s>

Introduction into the data structures used in LV2: Atoms, URIDs and URIs.

* **LV2_Atom**: Data containers with information about type and size.
    LV2_Atoms may have a body to store data of different types
    (like int, float, double, string, ..., and even other atoms).
    * **Vectors** are LV2_Atoms which contain an array of child data and information
        about the child type and the child size.
    * **Properties** are "named" LV2_Atoms which contain a key URID and data
        (e.g., as LV2_Atoms again).
    * **Objects** are LV2_Atoms which may take up multiple and/or different other property LV2_Atoms.
    * **Events** are prefixed with a time stamp.
    * And **Sequences** are LV2_atoms which can take up multiple events.
* **URI**: Unique identifier for each definition.
* **URID**: Serial number for an URI.
