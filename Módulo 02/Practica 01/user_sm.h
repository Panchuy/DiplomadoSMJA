/********************************************************
 *                                                      *
 *                                                      *
 *      user_sm.h			          	*
 *                                                      *
 *							*
 *		FI-UNAM					*
 *		17-2-2019                               *
 *                                                      *
 ********************************************************/

// State Machine 
void user_sm(float intensity, float *light_values, float *observations, int size, float laser_value, int  dest, int obs ,
					movement *movements  ,int *next_state ,float Mag_Advance ,float max_twist)
{

 int state = *next_state;
 int i;
 float izq=0,frente=0,der=0;
 int obstacle;
 static float d_x,d_y,orientacion;
 static	int bandera;

 printf("intensity %f\n",intensity);
 printf("quantized destination %d\n",dest);
 printf("quantized obs %d\n",obs);

 for(int i = 0; i < 8; i++)
        printf("light_values[%d] %f\n",i,light_values[i]);
 for (int i = 0; i < size ; i++ ) 
         printf("laser observations[%d] %f\n",i,observations[i]);

	for (int i=0;i<=15;i++){
		der+=observations[i];
		frente+=observations[i+16];
		izq+=observations[i+32];
	}

	der=der/16;
	frente=frente/16;
	izq=izq/16;

	if(frente== der && frente==izq)
		obstacle=0;
	else{
		if(frente< der && frente< izq)
			obstacle=3;
		else{
			if(der<laser_value){
				if((frente-izq)==0)
					obstacle=1;
				else
					obstacle=2;			
			}		
			else if(izq<laser_value){
				if((frente-der)==0)
					obstacle=5;
				else
					obstacle=4;			
			}		
		}
	}



	switch (state) {

        case 0:
		d_x=0;
		d_y=0;
		orientacion=0;
		
		if(obstacle==0){		
			*movements=generate_output(FORWARD,Mag_Advance,max_twist);
                	*next_state = 0;
			d_x=0;
			d_y=0;
			orientacion=0;
			bandera=0;
		}	
		else{
			*movements=generate_output(STOP,Mag_Advance,max_twist);
			if(obstacle==1)
                		*next_state = 1;
			else if(obstacle==2)
				*next_state = 5; 			
			else 	
				*next_state = 10; 
		}
	break;

        case 1:
               *movements=generate_output(BACKWARD,Mag_Advance/3,max_twist);
               *next_state = 2;
		d_x+=cos(orientacion)*(-Mag_Advance/3);
		d_y+=sin(orientacion)*(-Mag_Advance/3);

	break;

        case 2:
               *movements=generate_output(LEFT,Mag_Advance,max_twist);
               *next_state = 3;
		orientacion+=max_twist;
	break;
		
        case 3:
               *movements=generate_output(FORWARD,Mag_Advance,max_twist);
               *next_state = 4;
		d_x+=cos(orientacion)*(Mag_Advance);
		d_y+=sin(orientacion)*(Mag_Advance);

	break;

        case 4:
               *movements=generate_output(RIGHT,Mag_Advance,max_twist);
               *next_state = 14;
		orientacion+=-max_twist;
	break;

        case 5:
               *movements=generate_output(BACKWARD,Mag_Advance/3,max_twist);
               *next_state = 6;
		d_x+=cos(orientacion)*(-Mag_Advance/3);
		d_y+=sin(orientacion)*(-Mag_Advance/3);

	break;

        case 6:
               *movements=generate_output(LEFT,Mag_Advance,max_twist);
               *next_state = 7;
		orientacion+=max_twist;

	break;

        case 7:
               *movements=generate_output(LEFT,Mag_Advance,max_twist);
               *next_state = 8;
		orientacion+=max_twist;
	break;

        case 8:
               *movements=generate_output(FORWARD,Mag_Advance,max_twist);
               *next_state = 9;
		d_x+=cos(orientacion)*(Mag_Advance);
		d_y+=sin(orientacion)*(Mag_Advance);

	break;

        case 9:
               *movements=generate_output(RIGHT,Mag_Advance,max_twist);
               *next_state = 4;
		orientacion-=max_twist;
	break;

        case 10:
               *movements=generate_output(BACKWARD,Mag_Advance/3,max_twist);
               *next_state = 11;
		d_x+=cos(orientacion)*(-Mag_Advance/3);
		d_y+=sin(orientacion)*(-Mag_Advance/3);

	break;

        case 11:
               *movements=generate_output(LEFT,Mag_Advance,max_twist);
               *next_state = 12;
		orientacion+=max_twist;
	break;

        case 12:
               *movements=generate_output(LEFT,Mag_Advance,max_twist);
               *next_state = 13;
		orientacion+=max_twist;
	break;

        case 13:
		if(obstacle==0){
               		*movements=generate_output(RIGHT,Mag_Advance,max_twist);
			orientacion-=max_twist;
			*next_state = 24;
		}
		else if(obstacle==1){
               		*movements=generate_output(FORWARD,Mag_Advance,max_twist);
			d_x+=cos(orientacion)*(Mag_Advance);
			d_y+=sin(orientacion)*(Mag_Advance);
               		*next_state = 21;
		}
		else if(obstacle==2){
               		*movements=generate_output(BACKWARD,Mag_Advance/3,max_twist);
               		*next_state = 14;
			d_x+=cos(orientacion)*(-Mag_Advance/3);
			d_y+=sin(orientacion)*(-Mag_Advance/3);
		}
		else if(obstacle==3){
               		*movements=generate_output(BACKWARD,Mag_Advance/3,max_twist);
               		*next_state = 19;
			d_x+=cos(orientacion)*(-Mag_Advance/3);
			d_y+=sin(orientacion)*(-Mag_Advance/3);
		}
		else{
               		*movements=generate_output(RIGHT,Mag_Advance,max_twist);
               		*next_state = 18;
			orientacion-=max_twist;
		}
				
	break;

	case 14:	
        	*movements=generate_output(LEFT,Mag_Advance,max_twist);
        	*next_state = 15;
		orientacion+=max_twist;
	break;

	case 15:	
        	*movements=generate_output(LEFT,Mag_Advance,max_twist);
        	*next_state = 16;
		orientacion+=max_twist;
	break;

	case 16:	
        	*movements=generate_output(FORWARD,Mag_Advance,max_twist);
        	*next_state = 17;
		d_x+=cos(orientacion)*(Mag_Advance);
		d_y+=sin(orientacion)*(Mag_Advance);

	break;

	case 17:	
        	*movements=generate_output(RIGHT,Mag_Advance,max_twist);
        	*next_state = 18;
		orientacion-=max_twist;
	break;

	case 18:	
        	*movements=generate_output(RIGHT,Mag_Advance,max_twist);
        	*next_state = 21;
		orientacion-=max_twist;
	break;

	case 19:	
        	*movements=generate_output(LEFT,Mag_Advance,max_twist);
        	*next_state = 20;
		orientacion+=max_twist;
	break;

	case 20:	
        	*movements=generate_output(LEFT,Mag_Advance,max_twist);
        	*next_state = 21;
		orientacion+=max_twist;
	break;

	case 21:	
		*movements=generate_output(STOP,Mag_Advance,max_twist);
		if(fabs(d_x)< Mag_Advance*1.1 && fabs(d_y)<Mag_Advance*1.1){      	
			if(bandera==0){
				bandera++;
				*next_state=13;			
			}
			else        		
				*next_state = 22;
		}
		else{
			*next_state=13;
		}
	break;

	case 22:	
        	*movements=generate_output(LEFT,Mag_Advance,max_twist*2/3);
        	*next_state = 0;
	break;

	case 24:	
        	*movements=generate_output(FORWARD,Mag_Advance,max_twist);
        	*next_state = 21;
		d_x+=cos(orientacion)*(Mag_Advance);
		d_y+=sin(orientacion)*(Mag_Advance);

	break;

	default:
		//printf("State %d not defined used ", state);
            	*movements=generate_output(STOP,Mag_Advance,max_twist);
            	*next_state = 0;

	break;
                
 }
 printf("distx: %f\n", d_x);
 printf("disty: %f\n", d_y);

 printf("Next State: %d \n", *next_state);
	printf("%f \n",laser_value);

}



                 
