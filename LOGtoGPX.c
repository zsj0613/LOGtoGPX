/*                        LOGtoGPX                        */
/*                Alberto Santagostino  2016              */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#define MAXCOORD 25000
FILE *P, *Pw;

typedef struct
{
    int counter;
    float latitude;
    float longitude;
    float speed;
    char date[11];
    char time[9];
} coordinates;

int main()
{
    int i, j, Ncoord, voidlines=0;
    char namefile[96];
    char txtfile[100];
    char gpxfile[100];
    char buffer[100];
    coordinates list[MAXCOORD];

    /* Start */
    printf("Inserire il nome del file di log (*.txt)\t> ");
    scanf("%s",namefile);

    snprintf(txtfile, sizeof txtfile, "%s%s", namefile, ".txt");
    printf("\nTentando di aprire \"%s\"... ",txtfile);

    if(access(txtfile, F_OK ) != -1 )
        printf("Il file esiste!");
    else
    {
        printf("Il file NON esiste! Riprovare\n");
        return EXIT_FAILURE;
    }

    P = fopen(txtfile,"r");

    /* Count not-coord lines and skip */
    while(fgets(buffer, sizeof(buffer), P) != NULL)
        if ((buffer[0] == '\n')||(!isdigit(buffer[0])))
            voidlines++;

    rewind(P);
    for(i = 0; i<voidlines; i++)
        fgets(buffer, 100, P);

    /* Read coord from file */
    for(i = 0; fscanf(P,"%d,%f,%f,%f,%[^,],%s",&list[i].counter,&list[i].latitude,&list[i].longitude,&list[i].speed,list[i].date,list[i].time) != EOF; i++);
    Ncoord = i;
    fclose(P);

    printf("\n\nCoordinate lette:\n");
    printf("%s\t%3s\t\t%3s\t\t%s\t\t%s\t\t%s\n","#","LAT","LNG","SPEED","DATE","TIME");
    for(i=0; i < Ncoord; i++)
        printf("%d\t%f\t%f\t%.5f\t\t%s\t%s\n",list[i].counter,list[i].latitude,list[i].longitude,list[i].speed,list[i].date,list[i].time);


    /* Convert into GPX, single track */

    //while(*namefile!='\0')                                    // output file maiuscole
    //   *namefile = toupper((unsigned char)*namefile);

    snprintf(gpxfile, sizeof gpxfile, "%s%s", namefile, ".GPX");
    Pw = fopen(gpxfile, "w");

    fprintf(Pw,"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
    fprintf(Pw,"<gpx xmlns=\"http://www.topografix.com/GPX/1/1\" version=\"1.1\" creator=\"LOGtoGPX\">\">\n");
    fprintf(Pw,"\t<metadata>\n");
    fprintf(Pw,"\t\t<name>GPX_%s</name>\n",namefile);
    fprintf(Pw,"\t</metadata>\n");
    fprintf(Pw,"\t<trk>\n");
    fprintf(Pw,"\t\t<name>Traccia01</name>\n",namefile);
    fprintf(Pw,"\t\t<trkseg>\n");

    for(i = 0; i < Ncoord; i++)
    {
        fprintf(Pw,"\t\t<trkpt lat=\"%.5f\" lon=\"%.5f\">\n",list[i].latitude,list[i].longitude);
        fprintf(Pw,"\t\t\t\t<speed>%.2f</speed>\n",(list[i].speed/(3.6)));
        fprintf(Pw,"\t\t\t\t<time>%sT%sZ</time>\n",list[i].date,list[i].time);
        //<sat>8</sat>
        fprintf(Pw,"\t\t\t\t<name>%d</name>\n",list[i].counter);
        fprintf(Pw,"\t\t</trkpt>\n");
    }
    fprintf(Pw,"\t\t</trkseg>\n");
    fprintf(Pw,"\t</trk>\n");
    fprintf(Pw,"\</gpx>\n");

    fclose(Pw);

    printf("\n\nIl file *.gpx e' stato generato\n");

    system("pause>nul");
    return EXIT_SUCCESS;
}
