## ¿Qué es Onmusik?

Onmusik es una aplicación que permite hacer streaming de tu música subida a Dropbox o Google Drive. De este modo puedes tener todas las canciones de forma online en cualquier sitio en el que estés

## ¿Cómo es la aplicación?

La interfaz de Onmusik es muy sencilla. Es un reproductor de música que muestra los álbums y cada una de sus canciones subidas. Una imagen vale más que mil palabras: 

![Explicación de la interfaz](https://dl.dropboxusercontent.com/u/27757291/06_Explaining.png)

## ¿Qué formatos de música soporta?
Pues prácticamente todos. La aplicación se encarga de convertir los ficheros de música al formato Ogg, que es un formato contenedor libre y abierto, desarrollado y mantenido por la Fundación Xiph.Org que no está restringido por las patentes de software, y está diseñado para proporcionar una difusión de flujo eficiente y manipulación de multimedios digitales de alta calidad.

## Empezar a usar Onmusik

### Dar permisos a la aplicación para usar Dropbox o Google Drive

Al iniciar la aplicación por primera vez, la aplicación nos pedirá que nos loguemos en Dropbox para poder almacenar y acceder a las canciones a subir en la nube. 

![Petición de permisos en dropbox](https://dl.dropboxusercontent.com/u/27757291/07_Permissions.png)

Si aceptamos se nos abrirá una ventana del explorador web para logarnos. Una vez que accedamos a nuestra cuenta de dropbox, aparecerá la ventana siguiente:

![Permitir que la aplicación pueda acceder a dropbox](https://dl.dropboxusercontent.com/u/27757291/08_Allow.png)

Si pulsamos el botón **"Permitir"** se nos redirigirá a otra página. Se debe copiar el código (Ctrl+C)...

![Copiando el código obtenido de dropbox](https://dl.dropboxusercontent.com/u/27757291/09_Code.png)

... y pegarlo en el campo de texto de la aplicación Onmusik

![Copiando el texto](https://dl.dropboxusercontent.com/u/27757291/10_CodeInOnmusik.png)

**Por fin!** Ya podemos subir canciones!

### Subiendo discos por primera vez

La primera vez que accedamos al programa, se nos indicará cual es el botón para subir canciones. ¡Tan sencillo como eso!

![Primer inicio](https://dl.dropboxusercontent.com/u/27757291/01_Welcome.png)

Pulsando el botón, se muestra el explorador de archivos, en el que podemos seleccionar una canción (se subirán todas las del mismo directorio) o el directorio donde están todas las canciones de cada disco que queramos subir.

![Seleccionando el disco a subir](https://dl.dropboxusercontent.com/u/27757291/02_DiscUpload.png)

**Con un poco de paciencia**, se subirá el disco y al final se refrescará la lista de los álbumes. La aplicación muestra los discos de la siguiente forma

![Reproduciendo una canción](https://dl.dropboxusercontent.com/u/27757291/05_Playing.png)

### Ecualizando el audio
Desde la versión 1.2 se ha introducido un ecualizador de audio para aumentar los graves o los agudos de la salida de audio. Los controles que aparecen más a la izquierda modifican las frecuencias más bajas, mientras que los de la derecha modifican las más altas

![Mostrando el ecualizador](https://dl.dropboxusercontent.com/u/27757291/11_equalizer.png)

### Visualizando la letra de las canciones
Desde la versión 1.3 se ha introducido un visualizador de la letra de las canciones. Sólo hay que pulsar en el botón de la letra "A" y aparecerá la letra de la canción seleccionada en el momento

![Visualizando la letra de la música](https://dl.dropboxusercontent.com/u/27757291/12_Lyrics.png)

### Reproduciendo ficheros locales
Desde la versión 1.5 se ha introducido la opción de reproducir ficheros locales especificando el fichero por parámetro o mediante el icono de carpeta de la aplicación

### Añadiendo otros servidores
Desde la versión 1.6 se puede conectar con Google Drive también

### Visualizador mejorado
Desde la versión 1.7 se puede alternar el visualizador gráfico de barras con el visualizador de onda con pulsar doble click en su espacio.

![Ecualizador gráfico](https://dl.dropboxusercontent.com/u/27757291/13_BarVis.png)

###Conversión de CD's
Desde la versión 1.8 se pueden subir CD's de música directamente desde la aplicación. Onmusik se encarga de realizar los siguientes pasos de forma automatizada: 
- Extraer las pistas del CD a una carpeta temporal
- Buscar información de las pistas en el servicio freedb.org (para ello es necesario configurar el servicio cddb)
- Convertir las pistas extraídas al formato .ogg 
- Finalmente, subir las pistas del cd al servidor seleccionado

Para subir el cd, primero debemos pulsar en el icono creado a tal efecto:
![Icono de subida de cd's](https://dl.dropboxusercontent.com/u/27757291/14_uploadCD.png)

y luego seleccionar la unidad de cd en la que está insertado el disco de música que queremos convertir
![Seleccionando la unidad](https://dl.dropboxusercontent.com/u/27757291/15_SelectDrive.png)

para finalmente, seleccionar el servidor al que queremos subir nuestra música
![Seleccionando la unidad](https://dl.dropboxusercontent.com/u/27757291/17_selectUploadServer.png)

####Obtener información de CD's
La información de las pistas de los cd's no está codificada en el disco, por eso es necesario acceder a una base de datos que contenga todas las pistas de cada álbum publicado. Para ello es necesario generar un identificador de cd que se encarga de asociarlo con su información. Este identificador es generado por la aplicación Onmusik y se envía al servicio ofrecido por [freedb.org](http://www.freedb.org/)

Para que la aplicación obtenga los datos es necesario que envíe a "freedb.org" un usuario y un nombre de máquina. Puedes introducir lo que te apetezca, (no es un login ni nada por el estilo pero "freedb.org" requiere que se envíe algo). Estos datos los puedes introducir con la opción "Opciones CDDB" que se muestra en las imágenes del punto anterior.

![Introduciendo los datos para obtener info de freedb.org](https://dl.dropboxusercontent.com/u/27757291/16_UpdatecddbInfo.png)
