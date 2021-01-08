from PIL import Image
import numpy as np
import matplotlib.pyplot as plt

#load
#img_name = input("enter image name:")
#img = np.array( Image.open(img_name) )
img = np.array( Image.open("roses.jpg") )

if img.shape != (212,104,3):
	raise "wrong size: image must be of shape (212,104,3)"

bw = 1/3 * ( img[:,:,0] + img[:,:,1] + img[:,:,2] )

#threshold
black = 1/2 * ( img[:,:,1] + img[:,:,2] )
red = img[:,:,0] - 3*black
black = ~(black < 50)
red = ~(red > 30)

#display
fig = plt.figure(figsize=(12,4))
plt.subplot(1,3,1)
plt.title('black and white')
plt.imshow(bw, interpolation='nearest', cmap=plt.cm.gray)
plt.colorbar()
plt.subplot(1,3,2)
plt.title('black')
plt.imshow(~black, interpolation='nearest', cmap=plt.cm.Greys)
plt.colorbar()
plt.subplot(1,3,3)
plt.title('red')
plt.imshow(~red, interpolation='nearest', cmap=plt.cm.Reds)
plt.colorbar()
plt.show()

###convert
length = black.size

black = black.reshape(length)
red = red.reshape(length)

RED = str()
BLACK = str()
for i in range(int(length/8)):
    redbits = hex( np.packbits( red[i*8:(i+1)*8] )[0] )
    RED += redbits+','
    
    blackbits = hex( np.packbits( black[i*8:(i+1)*8] )[0] )
    BLACK += blackbits+','

#save
'''
f = open('black.txt', 'w')
f.write(BLACK)
f.close()

f = open('red.txt', 'w')
f.write(RED)
f.close()
'''

f = open('imagedata.c', 'w')
f.write("#include <avr/pgmspace.h>\n\n")
f.write("const unsigned char IMAGE_BLACK[] PROGMEM = {\n")
f.write(BLACK)
f.write("};\n")
f.write("const unsigned char IMAGE_RED[] PROGMEM = {\n")
f.write(RED)
f.write("};\n")
f.close()
