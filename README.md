# c_73_read_BMP_file

## DESCRIPTION

Read BMP file's information into a structure.

[Informations about BMP file structure](https://en.wikipedia.org/wiki/BMP_file_format)

### IMPLEMENTATION DETAILS

- read file name (or path) from command line argument

- make sure the file is a BMP image

- create a structure to save all the BMP header and DIB infromation

- read in all the BMP header information and print them on screen

- read pixel data and print the image into the terminal window (use 1 for black pixels and 0 for white pixels)
- change color of the black pixels and save the new BMP file
