import base64
from io import BytesIO
from itertools import chain
from math import ceil
from PIL import Image, ImagePalette

def toRomAddr(n):
    return n - 0x80010000 + 0x800

def readString(f, n, enc="ascii"):
    try:
        s = f.read(n).decode(enc)
        end = s.find("\0")
        return s if end == -1 else s[0:end]
    except:
        return "--"

def readCString(f, maxlen=255):
    data = bytearray()
    for i in range(maxlen):
        ch = ord(f.read(1))
        if ch == 0: break
        data.append(ch)
    return data.decode("ascii")

def loadStrings(f, addr, ct, slen, enc="ascii"):
    f.seek(toRomAddr(addr))
    return [readString(f, slen, enc).strip() for i in range(ct)]

def applyTransparency(img, clut):
    # for simplicity, just using first black value
    for i in range(16):
        if clut[i] == [0, 0, 0]:
            img.info["transparency"] = i
            break

def readImage(f, clut, w, h):
    assert w % 2 == 0
    img = Image.new("P", (w, h))
    data = bytearray(w * h)
    for i in range(0, w * h, 2):
        #two pixels per byte
        b = ord(f.read(1))
        data[i] = b & 0x0f
        data[i+1] = (b & 0xf0) >> 4
    applyTransparency(img, clut)
    img.palette = clutToPalette(clut)
    img.frombytes(bytes(data))
    return img

def loadImageRect(f, clut, x, y, w, h, stride, baseOfs=0):
    assert w % 2 == 0 and x % 2 == 0
    startOfs = baseOfs + y * stride + x // 2
    img = Image.new("P", (w, h))
    data = bytearray(w * h)
    for iy in range(0, h):
        f.seek(startOfs + iy * stride)
        for ix in range(0, w, 2):
            b = ord(f.read(1))
            data[iy*w+ix] = b & 0x0f
            data[iy*w+ix+1] = (b & 0xf0) >> 4
    applyTransparency(img, clut)
    img.palette = clutToPalette(clut)
    img.frombytes(bytes(data))
    return img

def toRGB(color, fullRange=True):
    if isinstance(color, bytes):
        color = int.from_bytes(color, 'little')

    r = (color & 0b0000000000011111)
    g = (color & 0b0000001111100000) >> 5
    b = (color & 0b0111110000000000) >> 10
    #t = (color & 0b1000000000000000) >> 15

    #TBD
    if fullRange:
        return [ceil(r / 31 * 255), ceil(g / 31 * 255), ceil(b / 31 * 255)]
    else:
        return [r << 3, g << 3, b << 3]

def readClut(f):
    return [toRGB(f.read(2)) for j in range(16)]

def clutToPalette(clut):
    flattened = bytes(chain.from_iterable(clut))
    #return ImagePalette.ImagePalette("RGB", palette=flattened, size=48)
    return ImagePalette.raw("RGB", flattened)

def imgToBase64(img, fmt="PNG", html=True):
    with BytesIO() as mem:
        img.save(mem, fmt)
        encoded = str(base64.b64encode(mem.getvalue()), "ascii")    
    if html:
        return f'<img src="data:image/{fmt.lower()};base64,{encoded}">'
    else:
        return encoded

def wrapTag(tag, s):
    return f"<{tag}>{s}</{tag}>"

def formatList(lst, tag, outerTag=None):
    s = ""
    for entry in lst:
        s += f"<{tag}>{entry}</{tag}>"
    return f"<{outerTag}>{s}</{outerTag}>" if outerTag else s

