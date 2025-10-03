A less fully featured program called kar that:
    Creates an archive of the specified files/directories
    Extracts files and directories from an existing kar file into the current working directory (overwriting any existing files with the same names).

Maintains a data-structure for multiple possible trees, not only in memory, but to and from a single, linear file.

Consider the following directory tree where each file starting with a capital D is a directory, and each file starting with f is a regular file.

```sh
❯ ls -R *
f0.txt

D2F5:
f1.txt D1

D2F5/D1:
f5.txt D2 f2.txt

D2F5/D1/D2:
f4.txt  f3.txt
```

<img width="1542" height="2889" alt="diagram" src="https://github.com/user-attachments/assets/6b727360-f46a-453c-ac87-619e22cd3aef" />

Suppose now we executed kar as follows within this directory:

$ kar create archive.kar f0.txt D2F5/

The resulting file’s bytes should represent the following structure to be a valid kar archive (we’ve assumed left-right ordering within each directory has been preserved, this is not necessarily the case).
Beginning of file (0x0)
Header for f0.txt
All of the contents of f0.txt
Header for D2F5
Header for f1.txt
Contents of f1.txt
Header for D1
Header for f5.txt
Contents of f5.txt
Header for D2
Header for f4.txt
Contents of f4.txt
Header for f3.txt
Contents of f3.txt
Header for f2.txt
Contents of f2.txt

A more general description of the format is as follows: regular files feature a header immediately followed by their contents. Directories only feature a header, but are immediately followed by the appropriate representations of the files contained within them. This can include directories, which will follow their format recursively i.e. to complete archiving a directory, means it should have a header followed by all of the representations of the files contained within it, before moving on to the next file in the same directory as the original directory (that we started with).

The data-structure consists of a linked list to represent all the files a user would like to archive (the command line arguments), and then, for every file that is a directory, that node will separately also link to another linked list for the contents of that directory and so on recursively. This should naturally lend itself to creating the file format we require. The order of the files within each directory will not affect our programs’ operation or the correctness overall, so we will just use whatever order the operating system provides the filenames to us in. This means, that the order from the example in 4 may be slightly different (files in the same directory, might be in a different order), but the structure of directory headers, followed by the file headers and data for that directory must be maintained.
Each node in our archive tree will represent one file (whether it is a directory or not), and will store some properties of this file, but not the actual contents. The contents are written after a regular file’s header, remembering that a directory has no contents in our scheme.
