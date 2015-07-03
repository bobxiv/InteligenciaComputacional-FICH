clear all;
k0 = 0.000839257;
Centroide0 = [6.9125, 3.1, 5.84687, 2.13125];
Varianza0 = [0.246094, 0.084375, 0.196865, 0.0571484];
k1 = 0.00109746;
Centroide1 = [5.52963, 2.62222, 3.94074, 1.21852];
Varianza1 = [0.101344, 0.0676543, 0.140933, 0.0318793];
k2 = 0.00116216;
Centroide2 = [5.006, 3.418, 1.464, 0.244];
Varianza2 = [0.121764, 0.142276, 0.029504, 0.011264];
k3 = 0.00109131;
Centroide3 = [6.23659, 2.85854, 4.80732, 1.62195];
Varianza3 = [0.11793, 0.0751101, 0.0850684, 0.0675669];
p0 = [23.3368, -6.61328, 7.0388, -2.56864, 5.28231];
p1 = [-22.4178, 8.18387, -2.05428, -18.294, 2.26521];

entrada = [5.1,3.5,1.4,0.2];

error0=0.5*sum(((entrada - Centroide0).^2)./Varianza0);
error1=0.5*sum(((entrada - Centroide1).^2)./Varianza1);
error2=0.5*sum(((entrada - Centroide2).^2)./Varianza2);
error3=0.5*sum(((entrada - Centroide3).^2)./Varianza3);
entrada2=[-1,error0,error1,error2,error3];

out1= umbral(dot(entrada2, p0))
out1= umbral(dot(entrada2, p1))
