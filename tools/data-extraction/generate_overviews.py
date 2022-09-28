from os import mkdir
from os.path import exists
#from PIL import Image, ImagePalette, ImageDraw

from vh.config import *
from vh.util import formatList, imgToBase64
from vh.misc_data import MiscData
from vh.unit_data import UnitData
from vh.item_data import ItemData
from vh.sprite_loader import SpriteLoader
from vh.anim_viewer import generateUnitAnimViewer


# what references to generate
GENERATE_ITEM_OVERVIEW = False
GENERATE_UNIT_OVERVIEW = False
GENERATE_UNIT_ANIMS = False
GENERATE_UNIT_ANIM_VIEWER = False

HTML_HEAD = """<!DOCTYPE html>
<html lang="en">
<head>
  <title>Vandal Hearts - Data Preview</title>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <style>
    body { background-color: #eee; }
	ul { margin: 0; padding: 0; list-style-position: inside; list-style-type: none; }
	table { border: solid 1px; border-collapse: collapse; }
	th { 
	  border: solid 1px;
	  font-size: x-small;
	  text-shadow: -1px 0 white, 0 1px white, 1px 0 white, 0 -1px white;
	  top: 0;
	  position: sticky;
    }
	td { border: solid 1px; padding:0.5em; text-align:center; }
  </style>
</head>
<body>
"""

HTML_FOOT = """
</body>
</html>"""

def formatItemName(n):
    if not (1 <= n <= 139):
        assert n == 0
        return "--"
    icon = ItemData.loadIcon(n)
    s = imgToBase64(icon)
    s += ItemData.names[n]
    return s

def formatSpellName(n):
    return MiscData.spellNames[n]
               
with open(PSX_BINARY, "rb") as rom:

    miscData = MiscData(rom)
    itemData = ItemData(rom)
    unitData = UnitData(rom)
    
    if GENERATE_UNIT_ANIMS:
        first, last = 1, 143
        spriteLoader = SpriteLoader(rom)

        if not exists("unit-sprites/"):
            mkdir("unit-sprites/")

        for i in range(first, last + 1):
            print(f"unit: {i}")
            spriteLoader.load(i)
            spriteLoader.strip.save(f"unit-sprites/{i}-strip.png")
            spriteLoader.spritesheet.save(f"unit-sprites/{i}-sheet.png")
            #spriteLoader.spritesheet.save("tmpSheet.webp", lossless=True, method=4)
            for j in range(spriteLoader.getUnitAnimationCount(i)):
                spriteLoader.exportUnitAnimation(j, filename=f"unit-sprites/{i}-{j}.webp")
        
        if GENERATE_UNIT_ANIM_VIEWER:
            generateUnitAnimViewer(first, last, spriteLoader, filename="unit-anim-view.html")
    
    if GENERATE_ITEM_OVERVIEW:
        with open("item-reference.html", "w") as html:
            html.write(HTML_HEAD)
            html.write("<table>\n")
            html.write(formatList(["#","Name","Type", "Cost", "Power", "DspPwr", "Spell", "Description"], "th", "tr"))
            html.write("\n")
            
            for itemId in range(139):
                cells = []
                cells.append(itemId)
                cells.append(formatItemName(itemId))
                if itemId < 101:
                    item = itemData[itemId]
                    cells.append(item.type)
                    cells.append(item.cost)
                    cells.append(item.power)
                    cells.append(item.displayPower)
                    cells.append(formatSpellName(item.spell))
                    cells.append(item.description)
                html.write(formatList(cells, "td", "tr"))
                html.write("\n")
                
            html.write("</table>")
            html.write(HTML_FOOT)
    
    if GENERATE_UNIT_OVERVIEW:
        with open("unit-reference.html", "w") as html:
            html.write(HTML_HEAD)
            html.write("<table>\n")
            html.write(formatList(["#","Portrait","Type", "Advtg", "Class", "Step", "MgcSus", "AilSus", "AtkRng", "Equipment", "MP", "Spells"], "th", "tr"))
            html.write("\n")
        
            for unitId in range(144):
                cells = []
                cells.append(unitId)
                portrait = unitData.loadUnitPortrait(unitId)
                cells.append(imgToBase64(portrait))
                
                unit = unitData[unitId]
                cells.append(f'<b>{unit.name}</b><br>{unit.typeName}')
                cells.append(unit.advantage)
                cells.append(unit.className)
                cells.append(f't:{unit.step}<br>r:{unit.travelRange}')
                cells.append(unit.mgcSus)
                cells.append(unit.ailSus)
                cells.append(unit.atkRange)
                equipment = [formatItemName(x) for x in unit.equipment]
                cells.append(formatList(equipment, "li", "ul"))
                cells.append(unit.mp)
                spells = [formatSpellName(x) for x in unit.spells]
                cells.append(formatList(spells, "li", "ul"))
                
                html.write(formatList(cells, "td", "tr"))
                html.write("\n")
                
            html.write("</table>")
            html.write(HTML_FOOT)
