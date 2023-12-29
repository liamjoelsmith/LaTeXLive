# LaTeXLive

Compile and render .tex file/s in real-time.

## Disclaimer

This has only been tested on an Ubuntu 18.04 environment.

## Dependencies

* Qt 5.9.5
* texlive (for pdfLaTeX)

## How to get set up?

1. Clone this repository
1. Navigate to the ```/src``` folder, and run ```qmake app.pro```
2. Run ```make```
3. Create a ```*.tex``` file within the ```/src``` directory.
4. Run ```./app [your_tex_file].tex```

