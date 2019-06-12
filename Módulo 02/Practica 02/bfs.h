/***********************************************
*                                              *
*      bfs.h                     *
*                                              *
*                                              *
*              Bio-Robotics Laboratory         *
*              UNAM, 2019                      *
*                                              *
*                                              *
************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ros/package.h>

int cola[250]={};
int camino[250]={};
int ic=0,fc=0,tc=0,opc=1;

void entra(int v){
  cola[fc] = v;
  fc++;  
}

void sale(){ic++;}

void print_cola(){
  printf("\nstack: ");
    for(int i=ic; i < fc; i++)
    {
      printf(" %d ",cola[i]);
    }
    printf("\n");
}

void bfs_algorithm(int D ,int L){
  
  int node_actual = D;
  
  printf("%d\n",node_actual);

  while(node_actual!= L){
    nodes[node_actual].flag='Y';
    for( int j = 0; j < nodes[node_actual].num_conections; j++){
      if(nodes[nodes[nodes[node_actual].conections[j].node].num_node].flag!='Y' && nodes[nodes[nodes[node_actual].conections[j].node].num_node].flag!='E'){
        entra(nodes[nodes[node_actual].conections[j].node].num_node);
        nodes[nodes[nodes[node_actual].conections[j].node].num_node].parent=node_actual;
        nodes[nodes[nodes[node_actual].conections[j].node].num_node].flag='E';
      }
    }
    node_actual=cola[ic];
    sale();
  }

  while(node_actual!=-1){
    camino[tc]=node_actual;
    node_actual=nodes[node_actual].parent;
    tc++;  
  }
    
}

int bfs(float rx ,float ry ,float lx ,float ly, char *world_name,step *steps )
{
   
  ic=0;
  fc=0;
  tc=0;
   //char archivo[]="../data/obstacles/obstacles.top";
    char archivo[150];
   int i;
     int start = 0;
   int goal = 0;
   //"../data/obstacles/obstacles.top";
   std::string paths = ros::package::getPath("simulator");
   strcpy(archivo,paths.c_str());
   strcat(archivo,"/src/data/");
   strcat(archivo,world_name);
   strcat(archivo,"/");
   strcat(archivo,world_name);
   strcat(archivo,".top");


   for(int i=0; i<200; i++)
   {
      nodes[i].flag='N';
      nodes[i].num_conections = 0;
      nodes[i].parent = -1;
      nodes[i].acumulado = 0;
   }
   num_nodes=read_nodes(archivo);
   //printf("NUmero de nodos #: %d \n",num_nodes);
   for(i = 1; i < num_nodes; i++)
   {
      if ( sqrt(pow( nodes[i].x - rx ,2) + pow( nodes[i].y - ry ,2)) < sqrt( pow( nodes[start].x - rx ,2) + pow( nodes[start].y - ry ,2)) )
      { //  printf("r-n : %d Distancia %f  Node x %f  node y %f   rx %f  ry%f \n",i,sqrt(pow( nodes[i].x - rx ,2) + pow( nodes[i].y - ry ,2)),nodes[i].x,nodes[i].y,rx,ry  );
        start = i;
      }
      if (sqrt(pow( nodes[i].x - lx ,2) + pow( nodes[i].y - ly ,2)) < sqrt(pow( nodes[goal].x - lx ,2) + pow( nodes[goal].y - ly ,2) ) )
        goal = i;
   }
   //for(int i=0; i<num_nodes; i++)
    //printNode(i);
    
  printf("S: %d \n",start);
    printf("G: %d \n",goal);

    bfs_algorithm(start,goal);

   //printf("Final Stack\n");
   //print_stack();

    int j=0,k=tc-1;

    for(int i=0; i <tc; i++){
      printf(" %d \t",camino[i]);
      steps[i].node = nodes[camino[k]].num_node;
      steps[i].x = nodes[camino[k]].x;
      steps[i].y = nodes[camino[k]].y;
      k--;
    }
    //printf("\n");


  return 0;
} 
