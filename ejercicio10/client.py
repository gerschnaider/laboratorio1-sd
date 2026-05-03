import grpc
import miniservicios_pb2
import miniservicios_pb2_grpc
import logging
import sys


def run(server_address):

    with grpc.insecure_channel(f'{server_address}:5101') as channel:
        stub = miniservicios_pb2_grpc.mini_serviciosStub(channel)
        params = miniservicios_pb2.arithmetic_parameters(
            operands=[10, 20, 30],
            operands_amount=3
        )
        response_add = stub.Addition(params)
        print(f"Resultado de la suma (10+20+30): {response_add.response}")
        

if __name__ == "__main__":
    logging.basicConfig()
    target_ip = sys.argv[1] if len(sys.argv) > 1 else 'localhost'
    run(target_ip)