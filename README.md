# Word path

Given a natural language dictionary in json format you should compute.

If a path exist between two file
The longest path
The shortest path
The mean of all path

# Your programme should accept those arguments

Black Red tree
Hash table (FNV1 ou CRC32)

```
Word path.

Usage:
wp [options] -f <dictionary.json> [words...]
wp -h | --help
wp --version

Options:
-h --help                   Show this screen.
-f --file=<dictionary.json> Use file dictionary.json as input
--mean print the mean distance of all path.
--distance=<k> print all couple of words with distance k
--max print the two words with the max distance
--min print the two words with the min distance
```

# Build

`gcc main.c graphs.c json_miniparser.c lexmap.c lists.c -o wp` (no makefile)

# Random remarks

- works only with ASCII encoded json files

- if no words are specified, process on whole dictionnary. It can take some time ...

- if only 1 word is specified, there will be comparison to empty list ...

- normal usage should be with various words from dictionnary.
