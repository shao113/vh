from PIL import Image, ImagePalette, ImageDraw

from vh.config import *
from vh.util import imgToBase64
from vh.sprite_loader import SpriteLoader

def generateUnitAnimViewer(first, last, spriteLoader, filename="unit-anim-view.html"):

    def checkerboard(blockSize=8, color1=(238,)*3, color2=(204,)*3):
        imageSize = blockSize * 2
        img = Image.new("P", (imageSize, imageSize))
        img.palette = ImagePalette.raw("RGB", bytes(color1 + color2))
        dc = ImageDraw.Draw(img)
        dc.rectangle((0, 0, blockSize-1, blockSize-1), fill=1)
        dc.rectangle((blockSize, blockSize, imageSize-1, imageSize-1), fill=1)
        return img

    def getLightCheckerboardUrl():
        b64 = imgToBase64(checkerboard(), "PNG", html=False)
        return f"""url("data:image/png;base64,{b64}")"""

    def getDarkCheckerboardUrl():
        b64 = imgToBase64(checkerboard(color1=(136,)*3, color2=(170,)*3), "PNG", html=False)
        return f"""url("data:image/png;base64,{b64}")"""

    def writeGridSvg(html, gridId, width, height, rng, color):
        html.write(f"""<svg id="{gridId}" class="grid-overlay" version="1.1" xmlns="http://www.w3.org/2000/svg" width="{width}px" height="{height}px">\n""")
        html.write(f"<style>text{{stroke:#fffa;stroke-width:3px;paint-order:stroke;fill:#000;font-family:monospace;font-weight:bold;dominant-baseline:hanging;font-size:12px;}} rect{{stroke:{color};fill:none;}}</style>\n")
        for i in rng:
            rect = spriteLoader.rects[i]
            x, y, w, h = rect[0], rect[1], rect[2] - rect[0], rect[3] - rect[1]
            html.write(f"""<text x="{x+2}px" y="{y+3}px">{i}</text>\n""")
            html.write(f"""<rect x="{x}px" y="{y}px" width="{w}px" height="{h}px" />\n""")
        html.write("</svg>\n")

    def writeRadioGroup(html, name, options, checked=0):
        html.write("<fieldset>\n")
        html.write(f"    <legend>{name}:</legend>\n")
        lname = name.lower()
        for i, option in enumerate(options):
            option, caption = option
            html.write("""    <div class="horiz">\n""")
            html.write(f"""        <input type="radio" id="{lname}-{option}" name="{lname}" value="{lname}-{option}" onchange="change{name}('{lname}-{option}')"{' checked' if i==checked else ''}>\n""")
            html.write(f"""        <label for="{lname}-{option}">{caption}</label>\n""")
            html.write("    </div>\n")
        html.write("</fieldset>\n")

    def writePanels(html):
        html.write(
f"""<div id="panels">
    <div id="sprite-panel">
        <h3>Sprites:</h3>
        <div class="container">
            <img id="sprite-strip" class="ss-background-chk-lt" src="unit-sprites/{first}-strip.png">
""")
        writeGridSvg(html, "strip-grid", 512, 48, range(35, 45), "#000")
        html.write(
f"""        </div>
        <div class="container">
            <img id="sprite-sheet" class="ss-background-chk-lt" src="unit-sprites/{first}-sheet.png">
""")
        writeGridSvg(html, "normal-grid", 256, 352, range(1, 35), "#000")
        writeGridSvg(html, "wide-grid", 256, 352, range(45, 65), "#000")
        html.write(
"""        </div>
    </div>
    <div id="anim-panel">
        <h3>Animations:</h3>
        <div id="animations"></div>
    </div>
</div>
""")

    with open(filename, "w") as html:
        html.write(
"""<!DOCTYPE html>
<html lang="en">
<head>
  <title>Vandal Hearts - View Unit Sprites</title>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <style>
    body { background-color: #eee; font-family: sans-serif; }
    fieldset { display: inline-block; margin: 1em; }
    label { user-select: none }
    .container { position: relative; }
    .panel { float: left; margin-right: 3em; }
    .horiz { display: inline-block; margin: 5px; }
    .ss-background-white { border: black solid 1px; background: #fff none; }
    .ss-background-pink { border: black solid 1px; background: #f0f none; }
""")
        html.write(
f"""    .ss-background-chk-lt {{ border: black solid 1px; background-image: {getLightCheckerboardUrl()};}}
    .ss-background-chk-dk {{ border: black solid 1px; background-image: {getDarkCheckerboardUrl()};}}
""")
        html.write(
"""    .grid-overlay { position: absolute; left: 0; top:0; margin: 1px; visibility: hidden; }
    .anim-box { background-color: black; margin-right: 1em; margin-bottom: 1em; color: white; text-align: center; float:left; }
    #panels { display: flex; }
    #anim-panel { flex-grow: 1; margin-left: 2em; min-width: 200px;  }
  </style>
  <script>
    let bgType = "background-chk-lt";
""")
        s = ','.join(f"{i}:{spriteLoader.getUnitAnimationCount(i)}" for i in range(first, last+1))
        html.write(f"    const unitAnimCounts = {{{s}}};")
        html.write(
"""
    function loadUnitSprites(unitId) {
        if (!(unitId in unitAnimCounts)) { alert("missing"); }
        const ct = unitAnimCounts[unitId];
        const strip = document.getElementById("sprite-strip");
        const sheet = document.getElementById("sprite-sheet");
        strip.src = `unit-sprites/${unitId}-strip.png`;
        sheet.src = `unit-sprites/${unitId}-sheet.png`;
        const container = document.getElementById("animations");
        container.textContent = '';
        for (let i = 0; i < ct; i++) {
            const animBox = container.appendChild(document.createElement('div'));
            animBox.className = "anim-box";
            animImg = animBox.appendChild(document.createElement('img'));
            animImg.className = "ss-background-chk-lt";
            animImg.src = `unit-sprites/${unitId}-${i}.webp`;
            animBox.appendChild(document.createElement('br'));
            animBox.appendChild(document.createTextNode(i.toString()));
        }
        changeBackground(bgType);
    }
    function togglePixelation() {
        document.body.style.imageRendering = document.body.style.imageRendering ? null : "pixelated";
    }
    function changeGrid(type) {
        const stripGrid = document.getElementById("strip-grid");
        const normalGrid = document.getElementById("normal-grid");
        const wideGrid = document.getElementById("wide-grid");
        stripGrid.style.visibility = "hidden";
        normalGrid.style.visibility = "hidden";
        wideGrid.style.visibility = "hidden";
        if (type == "grid-normal") {
            stripGrid.style.visibility = "visible";
            normalGrid.style.visibility = "visible";
        } else if (type == "grid-wide") {
            stripGrid.style.visibility = "visible";
            wideGrid.style.visibility = "visible";
        }
    }
    function changeBackground(type) {
        bgType = type;
        const images = document.getElementsByTagName('img');
        for (const image of images) {
            image.className = "ss-" + type;
        }
    }
  </script>
</head>
""")
        html.write(
f"""<body onload="loadUnitSprites({first})">
<fieldset>
    <legend>Unit:</legend>
    <select onchange="loadUnitSprites(this.options[this.selectedIndex].value)">
""")
        for i in range(first, last+1):
            html.write(f"""        <option value="{i}">{i}</option>\n""")
        html.write(
"""    </select>
</fieldset>
""")
        writeRadioGroup(html, "Grid", [
            ("none", "None"),
            ("normal", "Normal"), 
            ("wide", "Wide")])
        writeRadioGroup(html, "Background", [
            ("white", "White"), 
            ("pink", "Pink"), 
            ("chk-lt", "Checkerboard (Light)"), 
            ("chk-dk", "Checkerboard (Dark)")], checked=2)
        html.write(
"""<fieldset>
    <legend>Misc:</legend>
    <button onclick="togglePixelation()">Toggle pixelation</button>
</fieldset>
""")
        writePanels(html)
        html.write(
"""</body>
</html>
""")
        # I regret everything.
