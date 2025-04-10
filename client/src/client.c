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
	log_info(logger, "Hola, soy un log");
	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	valor = config_get_string_value(config,
									"CLAVE");
	ip = config_get_string_value(config,
								 "IP");
	puerto = config_get_string_value(config,
									 "PUERTO");

	// Loggeamos el valor de config

	log_info(logger, "La clave es: %s", valor);
	log_info(logger, "La clave es: %s", ip);
	log_info(logger, "La clave es: %s", puerto);

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

t_log *iniciar_logger(void)
{
	t_log *nuevo_logger;

	nuevo_logger = log_create("/home/utnso/Desktop/tp0/client/tp0",
							  "K.Y.A.R.I",
							  1,
							  LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	nuevo_config = config_create("/home/utnso/Desktop/tp0/client/cliente.config");

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
    char *leido;

    while (1)
    {
        leido = readline("> ");

        if (!leido) {
            break; // Si readline devuelve NULL, terminamos
        }

        log_info(logger, leido);  // Logueamos lo que se leyó

        add_history(leido);  // Agregamos a historial de la consola

        if (strcmp(leido, "") == 0) { // Si la línea está vacía
            free(leido);
            break;
        }

        printf("%s\n", leido);  // Mostramos en consola

        free(leido);  // Liberamos la memoria después de usarla
    }
}


void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char *leido;
	t_paquete *paquete;

	// Leemos y esta vez agregamos las lineas al paquete
	paquete = crear_paquete();
	leido = readline(">");
	while (strcmp(leido ,"") != 0) {
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		free(leido);
		leido = readline("> ");
    }
	
	enviar_paquete(paquete, conexion);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
	free(leido);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
