/*
*	Clement ZOTTI 11 074 801
*	Ferhat ABBAS 
*/

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include "utils.h"

#define __NO_STD_VECTOR			// Use cl::vector and cl::string and
#define __NO_STD_STRING			// not STL versions, more on this later
#include <CL/cl.h>

#define DATA_SIZE 8

const char *KernelSource =
"/************************************************\n"
"  Cette fonction effectue une substitution de\n"
"  caracteres en additionnant une valeur\n"
"************************************************/\n"
" char* ADD(char* mot, int cle)\n"
"{\n"
"   int taille,i;\n"
"\n"
"   taille = 7;  \n"
"   for (i = 0;  i < taille; i++)\n"
"       mot[i] = mot[i] + cle * i;\n"
"   return mot;\n"
"}\n"
"/************************************************\n"
"  Cette fonction effectue un ou exclusif\n"
"  entre la chaine de caracteres et la cle  \n"
"************************************************/\n"
"char* XOR(char* mot, char* cle)\n"
"{\n"
"    int taille,i;\n"
"    taille = 7;\n"
"   \n"
"    for (i = 0;  i < taille; i++)\n"
"        mot[i] = mot[i] ^ cle[i];\n"
"   return mot;\n"
"}\n"
"/*************************************************\n"
"  Cette fonction effectue un decalage circulaire \n"
"  vers la droite de \"nb\" caracteres\n"
"**************************************************/\n"
"char* decale(char* mot, int nb)\n"
"{\n"
"    char t[10];\n"
"	 int i;\n"
"    int max = 7 - 1;\n"
"    for(i = 0; i < nb; i++)\n"
"        t[i] = mot[max - i];\n"
"    for (i = max; i >= nb; i--)\n"
"        mot[i] = mot[i - nb];\n"
"    for (i = 0; i < nb; i++)\n"
"        mot[i] = t[nb - i -1];\n"
"\n"
"    return mot;\n"
"}\n"
"/*************************************************\n"
"  Cette fonction effectue une translation  \n"
"  entre les caracteres du mot d'une distance \n"
"  deduite de la cle\n"
"**************************************************/\n"
"char* echange(char* mot, int cle)\n"
"{\n"
"    int taille = 7;\n"
"    int i;\n"
"    for(i = 0; i + cle < taille; i++)\n"
"    { \n"
"        char temp;\n"
"        temp = mot[i];\n"
"        mot[i] = mot[i + cle];\n"
"        mot[i + cle] = temp;\n"
"    }\n"
"   return mot;\n"
"}\n"
"/*************************************************\n"
"  Cette fonction genere une cle numerique\n"
"**************************************************/\n"
"int getCle(char* mot)\n"
"{\n"
"    int cle,temp,i;\n"
"    temp =0;\n"
"    for (i = 0;i < 7; i++) \n"
"        temp = temp + (unsigned int) mot[i];\n"
"    cle = (temp % 4) + 1;\n"
"    return abs(cle);\n"
"}\n"
"/*************************************************\n"
"  Cette fonction encode le message\n"
"**************************************************/\n"
"char* encode(char* mot)\n"
"{\n"
"    char copie[8];\n"
"    int cle,j;\n"
"\n"
"    copie[0] = mot[0];\n"
"    copie[1] = mot[1];\n"
"    copie[2] = mot[2];\n"
"    copie[3] = mot[3];\n"
"    copie[4] = mot[4];\n"
"    copie[5] = mot[5];\n"
"    copie[6] = mot[6];\n"
"    copie[7] = mot[7];\n"
"    for (j = 0; j < 3; j++)\n"
"    {\n"
"        cle = getCle(mot);\n"
"        mot = decale(mot, cle / 2);\n"
"        mot = ADD(mot, cle);\n"
"        mot = echange(mot, cle);\n"
"        mot = XOR(mot, copie); \n"
"	 }\n"
"    return mot; }\n"
"\n"
"    __kernel void decoder( \n"
"	__global char* password, __global char* output, __global char* bound,__global int* foundCl)\n"
"{\n"
"   int idx = get_global_id(0);\n"
"	char* encodedPwd;\n"
"	char* encodedWord;\n"
"	char pwdCopy[8];\n"
"	(pwdCopy[0] = password[0]);\n"
"	(pwdCopy[1] = password[1]);\n"
"	(pwdCopy[2] = password[2]);\n"
"	(pwdCopy[3] = password[3]);\n"
"	(pwdCopy[4] = password[4]);\n"
"	(pwdCopy[5] = password[5]);\n"
"	(pwdCopy[6] = password[6]);\n"
"	(pwdCopy[7] = '\\0');\n"
"	encodedPwd = encode(pwdCopy);\n"
"	char a = 'a';\n"
"	char z = 'z';\n"
"	foundCl[0] = 0;\n"
"	char res[8];\n"
"	char word[8];\n"
"	for (word[0] = bound[idx]; word[0] <= bound[idx+1]; word[0]++)\n"
"	{\n"
"		if(foundCl[0]) {break;} \n"
"		for (word[1] = bound[idx]; word[1] <= z; word[1]++)\n"
"		{\n"
"			for (word[2] = bound[idx]; word[2] <= z; word[2]++)\n"
"			{\n"
"				 for (word[3] = bound[idx]; word[3] <= z; word[3]++)\n"
"				{\n"
"					for (word[4] = bound[idx]; word[4] <= z; word[4]++)\n"
"					{\n"
"						for (word[5] = bound[idx]; word[5] <= z; word[5]++)\n"
"						{\n"
"							for (word[6] = bound[idx]; word[6] <= z; word[6]++)\n"
"							{\n"
"								word[7] = '\\0';\n"
"								if(foundCl[0]){ break;} 			\n"
"								(res[0] = word[0]);\n"
"								(res[1] = word[1]);\n"
"								(res[2] = word[2]);\n"
"								(res[3] = word[3]);\n"
"								(res[4] = word[4]);\n"
"								(res[5] = word[5]);\n"
"								(res[6] = word[6]);\n"
"								(res[7] = word[7]);\n"
"								encodedWord = encode(res); //encoder le code\n"
" \n"
"								if ((encodedWord[0] == encodedPwd[0]) &&\n"
"									(encodedWord[1] == encodedPwd[1]) &&\n"
"									(encodedWord[2] == encodedPwd[2]) &&\n"
"									(encodedWord[3] == encodedPwd[3]) &&\n"
"									(encodedWord[4] == encodedPwd[4]) &&\n"
"									(encodedWord[5] == encodedPwd[5]) &&\n"
"									(encodedWord[6] == encodedPwd[6]) &&\n"
"									(encodedWord[7] == encodedPwd[7])) \n"
"								{\n"
"									//copier word dans output\n"
"									(output[0] = word[0]);\n"
"									(output[1] = word[1]);\n"
"									(output[2] = word[2]);\n"
"									(output[3] = word[3]);\n"
"									(output[4] = word[4]);\n"
"									(output[5] = word[5]);\n"
"									(output[6] = word[6]);\n"
"									(output[7] = word[7]);\n"
"									foundCl[0]= 1; \n"
"									return; \n"
"								}\n"
"							}\n"
"						}\n"
"					}\n"
"				}\n"
"			}\n"
"		}\n"
"	}\n"
"return;\n"
"}";


std::string limites(int nbThreads){
	int split,
		leftOver;

	std::string limits;
	std::string first,
		second;

	first = "a";

	for (auto i = 0; i < nbThreads; ++i) {
//		for (auto j = 0; j < nbThreads; ++j){
//			limits += first;
//			first[1]++;
//		}
		limits += first;
		first[0]++;
	}

	return limits;
}


int main(int argc, char* argv[]) {
	int devType = CL_DEVICE_TYPE_GPU;

	if (argc > 1) {
		devType = CL_DEVICE_TYPE_CPU;
		std::cout << "Using: CL_DEVICE_TYPE_CPU" << std::endl;
	}
	else {
		std::cout << "Using: CL_DEVICE_TYPE_GPU" << std::endl;
	}

	// Declaration of variables
	cl_int err;     // error code returned from api calls

	size_t global;  // global domain size for our calculation
	size_t local;   // local domain size for our calculation

	cl_platform_id cpPlatform; // OpenCL platform
	cl_device_id device_id;    // compute device id
	cl_context context;        // compute context
	cl_command_queue commands; // compute command queue
	cl_program program;        // compute program
	cl_kernel kernel;          // compute kernel

	// Connect to a compute device
	err = clGetPlatformIDs(1, &cpPlatform, NULL);
	if (err != CL_SUCCESS) {
		std::cerr << "Error: Failed to find a platform!" << std::endl;

		return EXIT_FAILURE;
	}

	// Get a device of the appropriate type - number of devices if defaulted to 1
	err = clGetDeviceIDs(cpPlatform, devType, 1, &device_id, NULL);
	if (err != CL_SUCCESS) {
		std::cerr << "Error: Failed to create a device group!" << std::endl;

		return EXIT_FAILURE;
	}

	// Create a compute context
	context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
	if (!context) {
		std::cerr << "Error: Failed to create a compute context!" << std::endl;

		return EXIT_FAILURE;
	}

	// Create a command queue
	commands = clCreateCommandQueue(context, device_id, 0, &err);
	if (!commands) {
		std::cerr << "Error: Failed to create a context's commands!" << std::endl;

		return EXIT_FAILURE;
	}

	// Create the compute program from the source
	program = clCreateProgramWithSource(context, 1,
		(const char **)&KernelSource,
		NULL, &err);
	if (!program) {
		std::cerr << "Error: Failed to create compute program!" << std::endl;

		return EXIT_FAILURE;
	}

	// Build the program executable
	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (err != CL_SUCCESS) {
		size_t len;
		char buffer[2048];

		std::cerr << "Error: Failed to build program executable!" << std::endl;
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG,
			sizeof(buffer), buffer, &len);
		std::cerr << buffer << std::endl;

		exit(1);
	}

	// Create the compute kernel in the program
	kernel = clCreateKernel(program, "decoder", &err);
	if (!kernel || err != CL_SUCCESS) {
		std::cerr << "Error: Failed to create compute kernel!" << std::endl;

		exit(1);
	}

	// Create data for the run - allocate memory
	const int nb_threads = 26;
	char* pwd = new char[DATA_SIZE];     // original data set given to device
	char* results = new char[DATA_SIZE]; // results returned from device
	auto lim = limites(nb_threads);
	char* lowerBound = new char[lim.size()+1];
	std::copy(lim.begin(), lim.end(), lowerBound);
	lowerBound[lim.size()] = '\0';
	int* found = new int[1];
	found[0] = 0;
	cl_mem password;                     // device memory used for the input array
	cl_mem output;                       // device memory used for the output array
	cl_mem limiteInf;                    // device memory used for the input array
	cl_mem limiteSup;                    // device memory used for the output array
	cl_mem foundCl;                    // device memory used for the output array

	// Initialize the vectors

	std::cout << "\nEntrez la chaine de caractere de longueur 7" << std::endl;
	std::cin >> pwd;
	std::cout << "Wait while processing..." << std::endl;


	// Create the device memory vectors
	//
	password = clCreateBuffer(context, CL_MEM_READ_ONLY,
		sizeof(char)* DATA_SIZE, NULL, NULL);
	output = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
		sizeof(char)* DATA_SIZE, NULL, NULL);
	limiteInf = clCreateBuffer(context, CL_MEM_READ_ONLY,
		(sizeof(char)* lim.size()), NULL, NULL);
	foundCl = clCreateBuffer(context, CL_MEM_READ_WRITE,
		sizeof(int), NULL, NULL);
	if (!password || !output || !limiteInf || !foundCl) {
		std::cerr << "Error: Failed to allocate device memory!" << std::endl;

		exit(1);
	}

	// Transfer the input vector into device memory
	err = clEnqueueWriteBuffer(commands, password,
		CL_TRUE, 0, sizeof(char)* DATA_SIZE,
		pwd, 0, NULL, NULL);
	if (err != CL_SUCCESS) {
		std::cerr << "Error: Failed to write to source array 1!" << std::endl;

		exit(1);
	}

	err = clEnqueueWriteBuffer(commands, limiteInf,
		CL_TRUE, 0, (sizeof(char)* lim.size()),
		lowerBound, 0, NULL, NULL);
	if (err != CL_SUCCESS) {
		std::cerr << "Error: Failed to write to source array 2!" << std::endl;

		exit(1);
	}

	err = clEnqueueWriteBuffer(commands, foundCl,
		CL_TRUE, 0, sizeof(int),
		found, 0, NULL, NULL);
	if (err != CL_SUCCESS) {
		std::cerr << "Error: Failed to write to source array 3!" << std::endl;

		exit(1);
	}



	// Set the arguments to the compute kernel
	err = 0;
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &password);
	err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
	err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &limiteInf);
	err |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &foundCl);
	if (err != CL_SUCCESS) {
		std::cerr << "Error: Failed to set kernel arguments! " << err << std::endl;

		exit(1);
	}

	{
		Timer t;
		// Execute the kernel over the vector using the 
		// maximum number of work group items for this device
		global = nb_threads - 1; // total number of work items
		err = clEnqueueNDRangeKernel(commands, kernel,
			1, NULL, &global, NULL,
			0, NULL, NULL);
		if (err) {
			std::cerr << "Error: Failed to execute kernel!" << std::endl;

			return EXIT_FAILURE;
		}

		// Wait for all commands to complete
		clFinish(commands);
	}
	// Read back the results from the device to verify the output
	//
	err = clEnqueueReadBuffer(commands, output,
		CL_TRUE, 0, sizeof(char)* DATA_SIZE,
		results, 0, NULL, NULL);
	if (err != CL_SUCCESS) {
		std::cerr << "Error: Failed to read output array! " << err << std::endl;

		exit(1);
	}
	err = clEnqueueReadBuffer(commands, foundCl,
		CL_TRUE, 0, sizeof(int),
		found, 0, NULL, NULL);
	if (err != CL_SUCCESS) {
		std::cerr << "Error: Failed to read output array! " << err << std::endl;

		exit(1);
	}

	// Print a brief summary detailing the results
	if (found[0]) {
		std::cout << "Le mot de passe trouve est : " << results << std::endl;
	}
	else {
		std::cout << "Le mot de passe n'as pas pu etre decrypte" << std::endl;
	}

	// Shutdown and cleanup
	delete[] pwd;
	delete[] results;
	delete[] lowerBound;
	delete[] found;

	clReleaseMemObject(password);
	clReleaseMemObject(output);
	clReleaseMemObject(limiteInf);
	clReleaseMemObject(foundCl);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(commands);
	clReleaseContext(context);
	system("pause");

	return 0;
}
