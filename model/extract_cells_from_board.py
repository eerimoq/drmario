import sys
from PIL import Image
from pathlib import Path

image_path = Path(sys.argv[1])
image_suffix = image_path.suffix
cells_path = Path(sys.argv[2])

with Image.open(image_path) as image:
    cell_width = image.width / 8
    cell_height = image.height / 16
    
    for row in range(16):
        upper = cell_height * row
        bottom = upper + cell_height
        
        for col in range(8):
            left = cell_width * col
            right = left + cell_width
            cell = image.crop((left, upper, right, bottom))
            cell.save(cells_path / f'{image_path.name}-{row}-{col}{image_suffix}')
