// Raytracer.cpp : main project file.

#include "stdafx.h"
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <stdlib.h>
#include <time.h>

#include "Vect.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Sphere.h"
#include "Object.h"
#include "Plane.h"
#include "Source.h"
#include "Cylinder.h"


using namespace std;

// guarda el color
struct RGBType{
	double r;
	double g;
	double b;

};
void savebmp(const char *filename ,int w,int h,int dpi, RGBType *data )
{
	FILE *f;
	int k = w*h;
	int s = 4 * k;
	int filesize = 54 + s;

	double factor = 39.375;
	int m = static_cast<int> (factor);
	int ppm = dpi*m;

	unsigned char bmpfileheader[14] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0 };
	unsigned char bmpinfoheader[40] = { 40, 0, 0, 0,  0, 0, 0, 0 ,0,0,0,0, 1,0,24,0 };

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize>>8);
	bmpfileheader[4] = (unsigned char)(filesize>>16);
	bmpfileheader[5] = (unsigned char)(filesize>>24);

	bmpinfoheader[4] = (unsigned char)(w);
	bmpinfoheader[5] = (unsigned char)(w>>8);
	bmpinfoheader[6] = (unsigned char)(w>>16);
	bmpinfoheader[7] = (unsigned char)(w>>24);

	bmpinfoheader[8] = (unsigned char)(h);
	bmpinfoheader[9] = (unsigned char)(h >> 8);
	bmpinfoheader[10] = (unsigned char)(h >> 16);
	bmpinfoheader[11] = (unsigned char)(h >> 24);

	bmpinfoheader[21] = (unsigned char)(s);
	bmpinfoheader[22] = (unsigned char)(s >> 8);
	bmpinfoheader[23] = (unsigned char)(s >> 16);
	bmpinfoheader[24] = (unsigned char)(s >> 24);

	bmpinfoheader[25] = (unsigned char)(ppm);
	bmpinfoheader[26] = (unsigned char)(ppm >> 8);
	bmpinfoheader[27] = (unsigned char)(ppm >> 16);
	bmpinfoheader[28] = (unsigned char)(ppm >> 24);

	bmpinfoheader[29] = (unsigned char)(ppm);
	bmpinfoheader[30] = (unsigned char)(ppm >> 8);
	bmpinfoheader[31] = (unsigned char)(ppm >> 16);
	bmpinfoheader[32] = (unsigned char)(ppm >> 24);

	f = fopen(filename, "wb");

	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);
		
	for (int i = 0; i < k; i++)
	{
		RGBType rgb = data[i];

		double red = (data[i].r) * 255;
		double green = (data[i].g) * 255;
		double blue = (data[i].b) * 255;
		unsigned char color[3] = { (int)floor(blue), (int)floor(green), (int)floor(red) };
		fwrite(color, 1, 3, f);

	}
	fclose(f);
}	
int thisone;
int winningObjectIndex(vector<double> object_intersections)
{
	/*
	
	
	
	
	*/




	// retornara el valor de la interseccion ganadora
	int index_of_minimum_value;


	// Prevenir calculo innecesario
	if (object_intersections.size() == 0)
	{
		//si no hay nada
		return -1;
	}
	else if (object_intersections.size() == 1)
	{
		if (object_intersections.at(0)>0)
		{
			// si esa interseccion es mayor a 0 entonces esa interseccion es nuestro index
			return 0;
		}
		else
		{
			 // sino la unica interseccion es negativa
			return -1;
		}
	}
	else
	{
		// hay mas de un objeto que es intersectado
		// tenemos que hallar el maximo valor en el vector

		double max = 0;
		for (int i = 0; i < object_intersections.size(); i++)
		{
			if (max < object_intersections.at(i))
			{
				max = object_intersections.at(i);
			}

		}
		// then starting from the maximum value fin the minimun positive value
		//entonces del inicio del maximo valor enconrara el minimo positivo valor 
		if (max > 0)
		{
			// solo queremos los puntos positivos de intersecciones
			for (int i = 0; i < object_intersections.size(); i++)
			{
				if (object_intersections.at(i)>0 && object_intersections.at(i) <= max  )
				{
					max = object_intersections.at(i);
					index_of_minimum_value = i;
				}
			}
			return index_of_minimum_value;
		}
		else {
			//si todas les intersecciones son negativas
			return -1;

		}




	}
}

Color getColorAt(Vect intersection_position, Vect intersecting_ray_direction, vector<Object*> scene_objects, int index_of_winning_object,vector<Source*> light_sources, double accuracy, double ambientlight)
{
	// esta funcion es la que nos implementara las sombras
	//nos devuelve el color sin aplicar nada
	Color winning_object_color = scene_objects.at(index_of_winning_object)->getColor();
	Vect winning_object_normal = scene_objects.at(index_of_winning_object)->getNormalAt(intersection_position);
	if (winning_object_color.getColorSpecial() == 2)
	{
		// tiene textura de ajedrez
		int square = (int)floor(intersection_position.getVectX()) + (int)floor(intersection_position.getVectZ());
		if ( (square % 2) == 0)
		{
			// sera negra por patron
			winning_object_color.setColorRed(0);
			winning_object_color.setColorGreen(0);
			winning_object_color.setColorBlue(0);
		}
		else
		{
			// blanco
			winning_object_color.setColorRed(1);
			winning_object_color.setColorGreen(1);
			winning_object_color.setColorBlue(1);
		}
	}

	//---------------------------------------------------------------------------------------
	// se escala con la luz del ambiente

	Color final_color = winning_object_color.colorScalar(ambientlight);

	if (winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1) {
		// reflecion de objetos con intensidad especular

		double dot1 = winning_object_normal.dotProduct(intersecting_ray_direction.negative());
		Vect scalar1 = winning_object_normal.vectMult(dot1);
		Vect add1 = scalar1.vectAdd(intersecting_ray_direction);
		Vect scalar2 = add1.vectMult(2);
		Vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
		Vect reflection_direction = add2.normalize();

		Ray reflection_ray(intersection_position, reflection_direction);

		/// determinamos que rayo intersecta primero

		vector<double> reflection_intersections;

		for (int reflection_index = 0; reflection_index < scene_objects.size(); reflection_index++) {
			reflection_intersections.push_back(scene_objects.at(reflection_index)->findIntersection(reflection_ray));
		}
		// el vector ganador en el arreglo de reflexiones
		int index_of_winning_object_with_reflection = winningObjectIndex(reflection_intersections);

		if (index_of_winning_object_with_reflection != -1) {
			// si la reflexion fallla todo
			if (reflection_intersections.at(index_of_winning_object_with_reflection) > accuracy) {
				// determinamos la posicion y la direccion en el punto de la interseccion con el rayo de reflexion
				// el rayo solo afecta los colores reflejados en algunos

				Vect reflection_intersection_position = intersection_position.vectAdd(reflection_direction.vectMult(reflection_intersections.at(index_of_winning_object_with_reflection)));
				Vect reflection_intersection_ray_direction = reflection_direction;
				// Llamada recursiva
				// se resuelve de manera recursiva ...
				Color reflection_intersection_color = getColorAt(reflection_intersection_position, reflection_intersection_ray_direction, scene_objects, index_of_winning_object_with_reflection, light_sources, accuracy, ambientlight);

				final_color = final_color.colorAdd(reflection_intersection_color.colorScalar(winning_object_color.getColorSpecial()));
			}
		}
	}
	//-----------------------------------------------------------------------------------------------


	
	for (int light_index = 0; light_index < light_sources.size();light_index++)
	{
		Vect ligth_direction = light_sources.at(light_index) -> getLightPosition().vectAdd(intersection_position.negative()).normalize();
		//calculamos el angulo
		float cosine_angle = winning_object_normal.dotProduct(ligth_direction);
	//si es positivo
		if (cosine_angle > 0)
		{ // checkea sombras
			bool shadowed = false;
			Vect distance_to_light = light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize();
			//distancia a la luz
			float distance_to_light_magnitude = distance_to_light.magnitude();
			/*creamos un nuevo rayo de la direccion de nuestra fuente de luz
			y vamos a ver si es que ese rayo tiene alguna interseccion entre ese punto 
			y si lo hace creamos sombra.
			*/
			Ray shadow_ray(intersection_position, light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize());
			//valores de las intersecciones
			vector<double> secondary_intersections;
			for (int object_index = 0; object_index < scene_objects.size() && shadowed==false; object_index++)
			{
				secondary_intersections.push_back(scene_objects.at(object_index)->findIntersection(shadow_ray));
			}
			for (int c = 0; c < secondary_intersections.size(); c++)
			{
				if (secondary_intersections.at(c)> accuracy)
				{
					// si es menor a la distancia de la luz

					if (secondary_intersections.at(c)<= distance_to_light_magnitude)
					{
						shadowed = true;
					}
					break;
				}
			}
			if (shadowed == false)
			{
				final_color = final_color.colorAdd(winning_object_color.colorMultiply(light_sources.at(light_index)->getLightColor()).colorScalar(cosine_angle));

				if (winning_object_color.getColorSpecial()>0 && winning_object_color.getColorSpecial()<=1)
				{
					// se va a referir a brillo tiene que estar entre [0-1]
					double dot1 = winning_object_normal.dotProduct(intersecting_ray_direction.negative());
					Vect scalar1 = winning_object_normal.vectMult(dot1);
					Vect add1 = scalar1.vectAdd(intersecting_ray_direction);
					Vect scalar2 = add1.vectMult(2);
					Vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
					Vect reflection_direction = add2.normalize();

					double specular = reflection_direction.dotProduct(ligth_direction);
					if (specular > 0)
					{
						specular = pow(specular, 10);
						final_color = final_color.colorAdd(light_sources.at(light_index)->getLightColor().colorScalar(specular*winning_object_color.getColorSpecial()));
					}
				}
			}
		}
	}

	return final_color.clip();

}

int main(array<System::String ^> ^args)
{
	
	clock_t t1, t2;
	t1 = clock();

	int dpi = 72;
	int width = 640;
	int height = 480;
	int n = width*height;

	// lopea en todos los pixeles en las intersecciones con los objetos
	// en cada interseccion trata de saber que color es
	RGBType *pixels = new RGBType[n];


	// profundidad del anti slising 
	int aadepth = 1;
	double aathreshold = 0.1;
	double aspectratio = (double)width / (double)height;
	double ambientlight = 0.2;
	double accuracy = 0.00000001;

	// estoy pintando toda la imagen, recorriendo cada uno de los pixeles
	Vect O(0, 0, 0);
	// derecha
	Vect X(1, 0, 0);
	// arriba
	Vect Y(0, 1, 0);
	//prof
	Vect Z(0, 0, 1);

	Vect new_sphere_location(2, 2, 0);
	//creando una camara
	//------------------------------------------------------------------
	// tenemos que definir todas sus coordenadas que vamos a usar 
	//donde va a ver
	Vect campos(3, 1.5, -4);

	Vect look_at(0, 0, 0);
	// diferencia entre la posicion de la camara y donde va a ver
	Vect diff_btw(campos.getVectX() - look_at.getVectX(), campos.getVectY() - look_at.getVectY(), campos.getVectZ() - look_at.getVectZ());
	// vector normalizado que esta en la posición negativa entre la diferencia entre la camara y el origen
	Vect camdir = diff_btw.negative().normalize();
	//producto cruz de la dirección de la camara.

	Vect camright = Y.crossProduct(camdir).normalize();
	Vect camdown = camright.crossProduct(camdir);
	Camera scene_cam(campos, camdir, camright, camdown);

	Color white_light(1.0, 1.0, 1.0, 0);
	// el cuarto valor agrega reflejos y brillo

	Color pretty_green(0.5, 1.0, 0.5, 0.3);
	Color maroon(0.5, 0.25, 0.25, 0);
	Color tile_floor(1, 1, 1, 2);
	Color gray(0.5, 0.5, 0.5, 0);
	Color black(0.0, 0.0, 0.0, 0);
	// posicion de la luz , creamos el vector

	Vect light_position(-7, 10, -10);
	Light scene_light(light_position, white_light);
	vector<Source*> light_sources;
	light_sources.push_back(dynamic_cast<Source*>(&scene_light));

	// objetos de la escena
	Sphere scene_sphere(O, 1, pretty_green);
	Sphere scene_sphere2(new_sphere_location, 0.5, maroon);
	Cylinder cylinder1(Vect(0, 0.5, -2), .5, 0.5, maroon);
	Plane scene_plane(Y, -1, tile_floor);
	// vector que contiene a los objetos de la escena
	vector<Object*> scene_objects;
	//scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));
	//scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere2));
	scene_objects.push_back(dynamic_cast<Object*>(&scene_plane));
	scene_objects.push_back(dynamic_cast<Object*>(&cylinder1));

	int thisone, aa_index;
	//valores que van a hacer que se vaya ligeramente a la derecha o a la izquierda
	double xamnt, yamnt;
	double tempRed, tempGreen, tempBlue;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			thisone = y*width + x;
			// se comienza sin anti-aliasing
			// el aspect ratio se refiere al aspecto de nuestra pantalla
			//1:1 se refiere a un cuadrado


			// se comienza con un pixer el blanco
			const int valor = aadepth*aadepth;

			vector<double> tempRed(valor);
			vector<double> tempGreen(valor);
			vector<double> tempBlue(valor);

			for (int aax = 0; aax < aadepth; aax++) {
				for (int aay = 0; aay < aadepth; aay++) {

					aa_index = aay*aadepth + aax;

					srand(time(0));

					// creamos el rayo de la camara a este pixel
					if (aadepth == 1) {

						// para crear rayos en las direcciones donde esta apuntando la camara
						//comenzamos sin anti alizing
						if (width > height) {
							// la imagen es mas ancha que su altura la corregimos
							xamnt = ((x + 0.5) / width)*aspectratio - (((width - height) / (double)height) / 2);
							yamnt = ((height - y) + 0.5) / height;
						}
						else if (height > width) {
							// la imagen es mas alta que el ancho
							xamnt = (x + 0.5) / width;
							yamnt = (((height - y) + 0.5) / height) / aspectratio - (((height - width) / (double)width) / 2);
						}
						else {
							// la imagen es un cuadrado
							xamnt = (x + 0.5) / width;
							yamnt = ((height - y) + 0.5) / height;
						}
					}
					else {
						// si hay anti aliasing
						if (width > height) {
							// la imagen es mas ancha que su altura la corregimos
							xamnt = ((x + (double)aax / ((double)aadepth - 1)) / width)*aspectratio - (((width - height) / (double)height) / 2);
							yamnt = ((height - y) + (double)aax / ((double)aadepth - 1)) / height;
						}
						else if (height > width) {
							// la imagen es mas alta que el ancho
							xamnt = (x + (double)aax / ((double)aadepth - 1)) / width;
							yamnt = (((height - y) + (double)aax / ((double)aadepth - 1)) / height) / aspectratio - (((height - width) / (double)width) / 2);
						}
						else {
							// la imagen es un cuadrado
							xamnt = (x + (double)aax / ((double)aadepth - 1)) / width;
							yamnt = ((height - y) + (double)aax / ((double)aadepth - 1)) / height;
						}
					}

					Vect cam_ray_origin = scene_cam.getCameraPosition();
					Vect cam_ray_direction = camdir.vectAdd(camright.vectMult(xamnt - 0.5).vectAdd(camdown.vectMult(yamnt - 0.5))).normalize();
					//para saber donde esta 
					// retorna el color
					/*The vector between the camera and the scene goes through the center of the image so xamnt and yamnt
					offset the direction vector away from the direction the camera is pointed in order to sweep rays through
					the pixels above and below the camera and to the left and to the right of the camera.The camera direction
					doesn't change, but xamnt and yamnt send rays through all the pixels the camera isn't pointed directly at,
					so they sweep through all the other pixels in the image plane.
					*/
					Ray cam_ray(cam_ray_origin, cam_ray_direction);

					vector<double> intersections;
					//crearemos un arreglo de intersecciones
					for (int index = 0; index < scene_objects.size(); index++) {
						intersections.push_back(scene_objects.at(index)->findIntersection(cam_ray));
					}
					// tenemos que encontrar el objeto que se encuentra mas cercano a la camara
					int index_of_winning_object = winningObjectIndex(intersections);

					if (index_of_winning_object == -1) {
						// pone el fondo negro
						tempRed[aa_index] = 0;
						tempGreen[aa_index] = 0;
						tempBlue[aa_index] = 0;
					}
					else{
						// la posición de unn objeto en nuestra scena
						//extraemos el color del objeto
						if (intersections.at(index_of_winning_object) > accuracy) {
							// determine the position and direction vectors at the point of intersection

							Vect intersection_position = cam_ray_origin.vectAdd(cam_ray_direction.vectMult(intersections.at(index_of_winning_object)));
							Vect intersecting_ray_direction = cam_ray_direction;

							Color intersection_color = getColorAt(intersection_position, intersecting_ray_direction, scene_objects, index_of_winning_object, light_sources, accuracy, ambientlight);

							tempRed[aa_index] = intersection_color.getColorRed();
							tempGreen[aa_index] = intersection_color.getColorGreen();
							tempBlue[aa_index] = intersection_color.getColorBlue();
						}
					}
				}
			}

			//el promedio de los pixeles
			double totalRed = 0;
			double totalGreen = 0;
			double totalBlue = 0;

			for (int iRed = 0; iRed < aadepth*aadepth; iRed++) {
				totalRed = totalRed + tempRed[iRed];
			}
			for (int iGreen = 0; iGreen < aadepth*aadepth; iGreen++) {
				totalGreen = totalGreen + tempGreen[iGreen];
			}
			for (int iBlue = 0; iBlue < aadepth*aadepth; iBlue++) {
				totalBlue = totalBlue + tempBlue[iBlue];
			}

			double avgRed = totalRed / (aadepth*aadepth);
			double avgGreen = totalGreen / (aadepth*aadepth);
			double avgBlue = totalBlue / (aadepth*aadepth);

			pixels[thisone].r = avgRed;
			pixels[thisone].g = avgGreen;
			pixels[thisone].b = avgBlue;
		}
	}

	savebmp("scene_anti-aliased.bmp", width, height, dpi, pixels);

	delete pixels, tempRed, tempGreen, tempBlue;

	t2 = clock();
	float diff = ((float)t2 - (float)t1) / 10000;

	printf("%d segundos\n",diff);
	getchar();
	return 0;
}
