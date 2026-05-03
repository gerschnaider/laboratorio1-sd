all: google/type/date.proto miniservicios_pb2_grpc.py miniservicios_pb2_grpc.py


miniservicios_pb2_grpc.py miniservicios_pb2.py:
	python -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. miniservicios.proto

google/type/date.proto:
	mkdir -p ./google/type && curl https://raw.githubusercontent.com/googleapis/googleapis/master/google/type/date.proto -o google/type/date.proto