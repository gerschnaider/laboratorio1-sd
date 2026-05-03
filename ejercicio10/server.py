import grpc
from concurrent import futures
import time
from datetime import datetime
import socket

# Importamos los archivos generados y los tipos de Google
import miniservicios_pb2
import miniservicios_pb2_grpc
from google.type import date_pb2
from google.protobuf import empty_pb2

class mini_servicios_Servicer(miniservicios_pb2_grpc.mini_serviciosServicer):
    def GetServerName(self, request, context):
        hostname = socket.gethostname()
        return miniservicios_pb2.server_name_return(name=hostname)

    def GetServerDate(self, request, context):
        now = datetime.now()
        google_date = date_pb2.Date(year=now.year, month=now.month, day=now.day)
        return miniservicios_pb2.Server_date_return(date = google_date)

    def IntegerToBinary(self, request, context):
        binary_string = bin(request.integer)[2:]  # Convertir a binario y eliminar el prefijo '0b'
        return miniservicios_pb2.binary_number(binary_number=binary_string)
    
    def Addition(self, request, context):
        resultado = sum(request.operands)
        return miniservicios_pb2.arithmetic_response(response=resultado)

    def Multiplication(self, request, context):
        resultado = 1
        for num in request.operands:
            resultado *= num
        return miniservicios_pb2.arithmetic_response(response=resultado)

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    miniservicios_pb2_grpc.add_mini_serviciosServicer_to_server(mini_servicios_Servicer(), server)
    server.add_insecure_port('[::]:5101')
    server.start()
    print("Servidor gRPC iniciado en el puerto 5101.")
    server.wait_for_termination()

if __name__ == '__main__':
    serve()