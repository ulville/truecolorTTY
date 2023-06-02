# truecolorTTY

Print true color images to linux console ([_the one Linus (LTT) fell into after deleting his whole GUI stack in PopOS_](https://youtu.be/0506yDSgU7M?t=760)).

The [linux console](https://en.wikipedia.org/wiki/Linux_console) is very limited in terms of color support by default. It only supports 16 foreground and 8 background colors.

However it supports setting different RGB color values to those 16 standard terminal colors using _OSC (Operating System Command) sequences_. Similar to setting a colorscheme for your regular terminal emulator. But it doesn't update the colors of already printed characters. Using this behavior as a hack we can print an image pixel by pixel with actual RGB colors.

It automatically scales down the image to fit into the console.

## Usage

### Print an image

```
./truecolorTTY -f /path/to/image.png
```

### Print HSV field

```
./truecolorTTY hsv
```

### Print RGB field

```
./truecolorTTY hsv
```