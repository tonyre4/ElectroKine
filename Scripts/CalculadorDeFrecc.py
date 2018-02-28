mode = 0

Pr0 = [1,8,64,256,1024]
Pr1 = [1,8,64,256,1024]
Pr2 = [1,8,32,64]
PrMax = [255,65535,255]
Prs = [Pr0,Pr1,Pr2]
Aclk = 16000000


def main():
    out = ""
    if mode == 0: #Para calcular corte
        #compare match register = [ 16,000,000Hz/ (prescaler * desired interrupt frequency) ] - 1
        #CMR = [Aclk / (Pr * DIF)] - 1

        while True:
            try:
                DIF = input("Desired frecc:")
                DIF = int(DIF)
                break
            except:
                print "This is not an integer number"

        for i,pmax in enumerate(PrMax):
            for j,p in enumerate(Prs[i]):
                CMR = ((Aclk) / (p * DIF)) - 1
                try:
                    RF = (float(Aclk))/(float(p) * float(CMR+1))
                except:
                    print "Error cannot be calculated"
                    RF = 0.0
                if CMR <= pmax and CMR >= 0:
                    print "For Pr" + str(i) + "= " + str(p) + " =>" + str(CMR) + "\t ERR=" + str(RF-float(DIF))
                    out += "void f" + str(DIF) +"hzT" + str(i) + "(){setTimer" + str(i) + "("+ str(CMR) +","+ str("0x0%d"%(j+1)) +");}\n"
            print ""

        if len(out) == 0:
            "Solution cannot be solved"
        else:
            print out

if __name__ == '__main__':
    while True:
        main()









