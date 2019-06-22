/********************************************************
 *                                                      *
 *                                                      *
 *      campos potenciales.h                 *
 *                                                      *
 *              *
 *    FI-UNAM         *
 *    17-2-2019                               *
 *                                                      *
 ********************************************************/
// State Machine 
//#define THRESHOLD 48
#define THRESHOLD 4


int campos_p(float intensity, float *light_values, float *observations, int size, float laser_value, int  dest, int obs ,
          movement *movements  ,int *next_state ,float Mag_Advance ,float max_twist){

int max_val_light=0;
int ld,li;
float alpha,fuerza_a;
float beta,fuerza_r;
float ang_t,fuerza_t;
int state = *next_state;
float izq=0,frente=0,der=0;
int min_val_laser=0, lado;
float promedio[3];
float promediot=0;
int flag=0;
float laser[size]={};

 printf("intensity %f\n",intensity);
 printf("quantized destination %d\n",dest);
 printf("quantized obs %d\n",obs);
for(int i = 0; i <8; i++)
      printf("light_values[%d] %f\n",i,light_values[i]);

for (int i=1;i<=7;i++){
  if(light_values[max_val_light]<light_values[i])
    max_val_light=i;    
  }

if(max_val_light==0)
    li=7;
  else
    li=max_val_light-1;

  if(max_val_light==7)
    ld=0;
  else
    ld=max_val_light+1;

printf("li %f\n",light_values[li]);
printf("ls %f\n",light_values[ld]);
printf("max %f\n",light_values[max_val_light]);





//Fuerza de atracción
if(light_values[ld]>light_values[li]){
    alpha=(2*3.1415/8)*float(max_val_light)+0.05;//ángulo más una desviación
    fuerza_a=(light_values[max_val_light]+light_values[ld])/2;//Regla de la fuerza de atracción
  }

  else if(light_values[ld]<light_values[li]){
    alpha=(2*3.1415/8)*float(max_val_light)-0.05;//ángulo más una desviación
    fuerza_a=(light_values[max_val_light]+light_values[ld])/2;//Regla de la fuerza de atracción
  }

  else{
    alpha=(2*3.1415/8)*float(max_val_light);
    fuerza_a=light_values[max_val_light];//Regla de la fuerza de atracción
  }

/*if(alpha<3.1415)//Si el ángulo es menor a 180° gira izquierda
  alpha=alpha;
else
  alpha=(2*3.1415-alpha)*-1;//Corregimos ángulo para giro derecho 
*/





//Fuerza de repulción

//Leemos los valores de los sensores 

//Para simulación quitar laser y usar obervations, quitando el if y el else

 for (int i = 0; i < size ; i++ ){ 
         //printf("laser observations[%d] %f\n",i,observations[i]);
          
          if(isnan(observations[i]))
            laser[i]=laser[i-1];
          else{
          if(observations[i]>laser_value)
            laser[i]=laser_value;
          else
            laser[i]=observations[i];
          }

         printf("laser [%d] %f\n",i,laser[i]);
}

  for (int i=0;i<=15;i++){
    der+=laser[i];
    frente+=laser[i+16];
    izq+=laser[i+32];
  }

promedio[0]=der/16;
promedio[1]=frente/16;
promedio[2]=izq/16;

if(promedio[0]<promedio[1] && promedio[0]<promedio[2])
  lado=0;

if(promedio[1]<promedio[0] && promedio[1]<promedio[2])
  lado=16;

if(promedio[2]<promedio[1] && promedio[2]<promedio[0])
  lado=32;

if(promedio[1]==promedio[2] && promedio[0]==promedio[1])//Comentar esto en simulador
  lado=0;

min_val_laser=lado;

for(int i=lado+1; i<=lado+15;i++){
  if(laser[min_val_laser]>laser[i])
    min_val_laser=i;
}

printf("Valor máximo laser %d\n",min_val_laser);

if(min_val_laser>24){
  beta=-min_val_laser*3.1415/size;
//beta=(360*3.1415/180)-min_val_laser*3.1415/size;
}
else{
  beta=(min_val_laser*3.1415/size);
//beta=(min_val_laser*3.1415/size)-(360*3.1415/180);
}

printf("Valor máximo laser %f\n",beta*360/(2*3.1425));

for(int i=0; i<3; i++)
  promediot+=promedio[i];

ang_t=alpha*(promediot/laser_value)/3+beta*(laser_value-promediot/3)*10;



if(laser_value-promediot/3<0)
  fuerza_r=0;
else if ((promediot/laser_value)/3>0.97)
  fuerza_r=0;
else
  fuerza_r=-2/((promediot/laser_value)/3);

if(ang_t<3.1415)//Si el ángulo es menor a 180° gira izquierda
  ang_t=ang_t;
else
  ang_t=(2*3.1415-ang_t)*-1;//Corregimos ángulo para giro derecho


fuerza_a=0.75+(1/((fuerza_r+5)*5))*(6/fuerza_a)*((promediot/laser_value)/3);//Regla de la fuerza de atracción


fuerza_t=fuerza_a+fuerza_r;


printf("relacion %f\n",(promediot/laser_value)/3);
printf(" Alpha %f\n",alpha*((promediot/laser_value)/3)*360/(2*3.1415));
printf("BETA %f\n",beta*(laser_value-promediot)*10*360/(2*3.1415));

printf("Angulo total %f\n",ang_t*360/(2*3.1415));

printf("Fuerza atraccion %f\n",fuerza_a);
printf("Fuerza repulsión %f\n",fuerza_r);
printf("Fuerza total %f\n",fuerza_t);

printf("resta %f\n",laser_value-promediot/3);





switch (state) {


    case 0:
      if(intensity<THRESHOLD){//Si aún no llega a la luz sigue girando
        if(ang_t>0){   
          *movements=generate_output(LEFT,0,ang_t);
        }
        else{
          *movements=generate_output(RIGHT,0,fabs(ang_t));//Gira a la derecha con el valor absoluto del ángulo 
        }
        *next_state=1;
      }
      else//Si  ya llego ala luz va al estado 2 que es STOP
        *next_state = 2;
    
    break;
  
    case 1:

    if(fuerza_t>0){
      *movements=generate_output  (FORWARD,fuerza_t*Mag_Advance,0);
      *next_state=0;
    }
    
    else{
      *movements=generate_output  (BACKWARD,fabs(fuerza_t)*Mag_Advance,0);
      *next_state=3;
    }

    break;
    
    case 2:
      *movements=generate_output  (STOP,fuerza_a*Mag_Advance,0);
      *next_state = 0;
       flag=1;
      break;      

    case 3:
      if(ang_t>0)
          *movements=generate_output(LEFT,0,ang_t);
      else
          *movements=generate_output(RIGHT,fabs(ang_t),0);//Gira a la derecha con el valor absoluto del ángulo 
          
        *next_state=4;
    break;

    case 4:
            *movements=generate_output(FORWARD,fabs(fuerza_t)*Mag_Advance*0.0025,0);
          //*movements=generate_output(FORWARD,fabs(fuerza_t)*Mag_Advance*2.5,0); //SImulador
          *next_state=0;
    break;
      
    default:
      //printf("State %d not defined used ", state);
      *movements=generate_output(STOP,Mag_Advance,max_twist);
      *next_state = 0;

     break;            
  }

  return flag;
}