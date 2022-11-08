# Hash-Probe

Implementing a phone book using a hash table. This method deals with hash function collisions by hash probing (e.g., linear, quadratic). 

The backbone of the hash table is an array of which elements are hash table items. 
When you create a new instance of hash table, the size 64 (INIT_TABLE_SIZE) has been made and ready to be filled in. 
Each item is consisted of key, value, and Boolean indicators whether it is empty/removed or not. The “link” pointer is never used.
