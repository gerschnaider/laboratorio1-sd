import grpc
import miniservicios_pb2
import miniservicios_pb2_grpc
import logging
import time
import sys


def run(server_address):

    with grpc.insecure_channel(f'{server_address}:5101') as channel:
        stub = miniservicios_pb2_grpc.mini_serviciosStub(channel)
        params = miniservicios_pb2.arithmetic_parameters(
            operands=[10, 20, 30],
            operands_amount=3
        )
        inicio_ns = time.perf_counter_ns()
        response_add = stub.Addition(params)
        fin_ns = time.perf_counter_ns()
        tiempo_total_ns = fin_ns - inicio_ns
        
        print(f"Resultado de la suma (10+20+30): {response_add.response}")
        print(f"Tiempo de ejecución RPC: {tiempo_total_ns} nanosegundos")
        

if __name__ == "__main__":
    logging.basicConfig()
    target_ip = sys.argv[1] if len(sys.argv) > 1 else 'localhost'
    run(target_ip)