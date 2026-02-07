from json import loads
from struct import pack

#Endian support only for little endian

with open("Assets/frames.json", "r") as file:
  frames = loads(file.read())

