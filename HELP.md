# Line Editor Commands Help

* **display**: Prints the full text document with numerical line prefixes.
  * *Example*: `display`
* **insert [line_number] [text]**: Inserts text directly at that index, pushing others down.
  * *Example*: `insert 1 Hello World`
* **delete [line_number]**: Removes the line at the designated placement index.
  * *Example*: `delete 2`
* **save [filename.txt]**: Saves the current active document to disk.
  * *Example*: `save note.txt`
* **load [filename.txt]**: Restores a document saved to disk into memory.
  * *Example*: `load note.txt`
* **stats**: Prints out simple document calculations.
  * *Example*: `stats`
* **exit**: Shuts down the software.