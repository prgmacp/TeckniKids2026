import pygame
import serial
import time

# --- CONFIGURACIÓN ---
PUERTO = 'COM7' 
BAUDIOS = 9600

# Intentar conectar al Bluetooth
try:
    bt = serial.Serial(PUERTO, BAUDIOS)
    print(f"Conectado exitosamente al carrito en {PUERTO}")
except Exception as e:
    print(f"Error conectando al Bluetooth: {e}")
    exit()

# Inicializar Pygame y el Control
pygame.init()
pygame.joystick.init()

if pygame.joystick.get_count() == 0:
    print("No se detectó ningún control. Conecta el PS4 y reinicia.")
    exit()

joystick = pygame.joystick.Joystick(0)
joystick.init()
print(f"Control detectado: {joystick.get_name()}")
print("Usa el JOYSTICK IZQUIERDO para mover, X para abrir garra, Círculo para cerrar.")

# --- BUCLE PRINCIPAL ---
comando_actual = 'S'

def enviar_comando(comando):
    global comando_actual
    # Solo envía la letra si es diferente a la anterior para no saturar el Bluetooth
    if comando != comando_actual: 
        bt.write(comando.encode())
        print(f"Enviando señal: {comando}")
        comando_actual = comando

ejecutando = True

while ejecutando:
    for evento in pygame.event.get():
        if evento.type == pygame.QUIT:
            ejecutando = False

        # Mantener los botones de la Garra intactos
        if evento.type == pygame.JOYBUTTONDOWN:
            if evento.button == 0:  
                enviar_comando('A') # Botón X 
            elif evento.button == 1:  
                enviar_comando('C') # Botón Círculo 

    # --- LECTURA DEL JOYSTICK ANALÓGICO ---
    # Pygame lee los ejes del -1.0 al 1.0
    # Eje 0 = Izquierda/Derecha | Eje 1 = Arriba/Abajo
    eje_x = joystick.get_axis(0)
    eje_y = joystick.get_axis(1)
    
    # Zona muerta: Ignora cualquier valor menor a 0.5 para evitar movimientos fantasma
    zona_muerta = 0.5 

    if eje_y < -zona_muerta:
        enviar_comando('F') # Palanca hacia Arriba
    elif eje_y > zona_muerta:
        enviar_comando('B') # Palanca hacia Abajo
    elif eje_x < -zona_muerta:
        enviar_comando('L') # Palanca hacia la Izquierda
    elif eje_x > zona_muerta:
        enviar_comando('R') # Palanca hacia la Derecha
    else:
        enviar_comando('S') # Palanca en el Centro (Soltar)

    time.sleep(0.05) 

# Cerrar conexiones al salir
bt.close()
pygame.quit()