import math

ishex = True

##global samples, bits_out, DAC_r, values, twoPirad, step, typedir, out

def main():
    samples = 100
    bits_out = 8
    DAC_r = 8
    DAC_max = (2**DAC_r) - 2
    sine = True
    arduinoPrinter = True

    if sine:
        values = []
        twoPirad = math.pi*2
        step = twoPirad/float(samples)
        for i in range(samples):
            values.append(int((((math.sin(i*step))+1)/2)*DAC_max))
        print len(values)

    if arduinoPrinter:
        typedir = {8 : "char",
                   16: "short"}
        out = str(typedir[bits_out]) + " var[SAM] = {"

        for i,x in enumerate(values):
            if i==50: break
            if ishex:
                out += str(hex(x-127)) + ", "
            else:
                out += str(x-127) + ", "
        out = out[:-2] + "};"
        print "#define SAM " + str(samples)
        print out

if __name__ == "__main__":
    main()
