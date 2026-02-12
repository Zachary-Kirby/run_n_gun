from json import loads
from struct import pack,calcsize

"""
Sprite definition list:
  2 bytes: atlas width
  2 bytes: atlas height
  2 bytes: number of sprite definitions
  2 bytes: size of each sprite definition

Sprite definition:
  2 bytes: sprite ID
  2 bytes: x
  2 bytes: y
  2 bytes: width
  2 bytes: height
  2 bytes: duration

Animation definition list:
  2 bytes: number of animation definitions

Animation definition:
  1 bytes: length of name
  N bytes: name
  2 bytes: number of frames
  2N bytes: frame ID
"""

#Endian support only for little endian

try:
  file = open("Assets/frames.json", "r")
  frames = loads(file.read())
  file.close()
except FileNotFoundError as e:
  print("Error loading frames.json: " + str(e))
  exit(1)

meta = frames["meta"]
frames = frames["frames"]

#max of 256 byte long strings
def stringToBytes(string: str):
  return pack("B", len(string)) + string.encode()

"""
I want to output a sprite definition list
I want to output a animation definition list
"""
spriteDefinitions = []
spriteAnimations = {}

spriteID = 0
for frame in frames:
  spriteDefinitions += [[
    spriteID,
    frame["frame"]["x"],
    frame["frame"]["y"],
    frame["frame"]["w"],
    frame["frame"]["h"],
    frame["duration"]
  ]]
  nameData = str(frame["filename"]).split(':')
  if not spriteAnimations.get(nameData[0], False): spriteAnimations[nameData[0]] = []
  spriteAnimations[nameData[0]] += [spriteID]
  spriteID += 1

if spriteID > 65535:
  print("Too many sprites, max is 65535")
  exit(1)

binaryData = b""
binaryData += pack("<H", meta["size"]["w"])
binaryData += pack("<H", meta["size"]["h"])
binaryData += pack("<H", len(spriteDefinitions))
binaryData += pack("<H", calcsize("HHHHHH"))
for frame in spriteDefinitions:
  binaryData += pack("<HHHHHH", *frame)
binaryData += pack("<H", len(spriteAnimations))
for name, animation in spriteAnimations.items():
  binaryData += stringToBytes(name)
  binaryData += pack("<H", len(animation))
  for frame in animation:
    binaryData += pack("<H", frame)

with open("Assets/spriteData.bin", "wb") as file:
  file.write(binaryData)