#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	log_info(logger, "Hola! Soy un log");

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	valor = config_get_string_value(config, "CLAVE");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
    
    /*if (valor == NULL) {
        printf("La clave 'CLAVE' no existe en el archivo de configuración\n");
        abort();
    }*/

    log_info(logger, "El valor de CLAVE es: %s", valor);

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// Loggeamos el valor de config


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	/*t_log* nuevo_logger;

	return nuevo_logger;*/

	t_log* nuevo_logger = log_create("tp0.log", "CLIENTE", true, LOG_LEVEL_INFO);

    if (nuevo_logger == NULL) {
        printf("No se pudo crear el logger\n");
        abort();
    }

    return nuevo_logger;
}

t_config* iniciar_config(void)
{
	/*t_config* nuevo_config;

	return nuevo_config;*/

	t_config* nuevo_config = config_create("cliente.config");

    if (nuevo_config == NULL) {
        printf("No se pudo leer el archivo de configuración\n");
        abort();
    }

    return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido = NULL;

	// La primera te la dejo de yapa
	//leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío


	// ¡No te olvides de liberar las lineas antes de regresar!

    leido = readline("> ");
	log_info(logger, "Entrada: %s", leido); 

	while(strcmp(leido, "") != 0){  
		free(leido);
        leido = readline("> ");
        log_info(logger, "Entrada: %s", leido); 
    }
	free(leido);

	/* Otra forma  (en esta forma no cargo en el logger el ENTER)

	while(true){  //un bucle para poder escribir la cantidad de entradas que querramos
        leido = readline("> "); // Espera entrada del usuario
        
        if(strcmp(leido, "") == 0){ // Si la entrada está vacía, salir del bucle (osea que con ENTER termina el bucle)
            free(leido);
            break;
        }

        log_info(logger, "Entrada: %s", leido); // Loguea la entrada
        free(leido); // Libera la memoria asignada por readline
    }
	*/

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete

	leido = readline("> ");

	while(strcmp(leido, "") != 0){  
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		free(leido);
        leido = readline("> ");
    }
	free(leido);
	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
