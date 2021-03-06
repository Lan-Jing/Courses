# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: file_server.proto
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='file_server.proto',
  package='',
  syntax='proto3',
  serialized_options=None,
  create_key=_descriptor._internal_create_key,
  serialized_pb=b'\n\x11\x66ile_server.proto\"H\n\tupRequest\x12\x0c\n\x04path\x18\x01 \x01(\t\x12\x0e\n\x06\x62uffer\x18\x02 \x01(\x0c\x12\n\n\x02id\x18\x03 \x01(\x05\x12\x11\n\tcascading\x18\x04 \x01(\x05\"\'\n\x0b\x64ownRequest\x12\x0c\n\x04path\x18\x01 \x01(\t\x12\n\n\x02id\x18\x02 \x01(\x05\"7\n\tstringMes\x12\x0b\n\x03str\x18\x01 \x01(\t\x12\n\n\x02id\x18\x02 \x01(\x05\x12\x11\n\tcascading\x18\x03 \x01(\x05\")\n\tbufferMes\x12\x0e\n\x06\x62uffer\x18\x01 \x01(\x0c\x12\x0c\n\x04\x64one\x18\x02 \x01(\x05\"\x18\n\x08\x66s_reply\x12\x0c\n\x04\x64one\x18\x01 \x01(\x05\"\x10\n\x02id\x12\n\n\x02id\x18\x01 \x01(\x05\x32\xf2\x01\n\x0b\x66ile_server\x12\x1f\n\x03pwd\x12\n.stringMes\x1a\n.stringMes\"\x00\x12\x17\n\x02ls\x12\x03.id\x1a\n.stringMes\"\x00\x12\x1d\n\x02\x63\x64\x12\n.stringMes\x1a\t.fs_reply\"\x00\x12 \n\x05mkdir\x12\n.stringMes\x1a\t.fs_reply\"\x00\x12\x1d\n\x02rm\x12\n.stringMes\x1a\t.fs_reply\"\x00\x12!\n\x06upload\x12\n.upRequest\x1a\t.fs_reply\"\x00\x12&\n\x08\x64ownload\x12\x0c.downRequest\x1a\n.bufferMes\"\x00\x62\x06proto3'
)




_UPREQUEST = _descriptor.Descriptor(
  name='upRequest',
  full_name='upRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='path', full_name='upRequest.path', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='buffer', full_name='upRequest.buffer', index=1,
      number=2, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=b"",
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='id', full_name='upRequest.id', index=2,
      number=3, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='cascading', full_name='upRequest.cascading', index=3,
      number=4, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=21,
  serialized_end=93,
)


_DOWNREQUEST = _descriptor.Descriptor(
  name='downRequest',
  full_name='downRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='path', full_name='downRequest.path', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='id', full_name='downRequest.id', index=1,
      number=2, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=95,
  serialized_end=134,
)


_STRINGMES = _descriptor.Descriptor(
  name='stringMes',
  full_name='stringMes',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='str', full_name='stringMes.str', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='id', full_name='stringMes.id', index=1,
      number=2, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='cascading', full_name='stringMes.cascading', index=2,
      number=3, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=136,
  serialized_end=191,
)


_BUFFERMES = _descriptor.Descriptor(
  name='bufferMes',
  full_name='bufferMes',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='buffer', full_name='bufferMes.buffer', index=0,
      number=1, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=b"",
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='done', full_name='bufferMes.done', index=1,
      number=2, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=193,
  serialized_end=234,
)


_FS_REPLY = _descriptor.Descriptor(
  name='fs_reply',
  full_name='fs_reply',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='done', full_name='fs_reply.done', index=0,
      number=1, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=236,
  serialized_end=260,
)


_ID = _descriptor.Descriptor(
  name='id',
  full_name='id',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='id', full_name='id.id', index=0,
      number=1, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=262,
  serialized_end=278,
)

DESCRIPTOR.message_types_by_name['upRequest'] = _UPREQUEST
DESCRIPTOR.message_types_by_name['downRequest'] = _DOWNREQUEST
DESCRIPTOR.message_types_by_name['stringMes'] = _STRINGMES
DESCRIPTOR.message_types_by_name['bufferMes'] = _BUFFERMES
DESCRIPTOR.message_types_by_name['fs_reply'] = _FS_REPLY
DESCRIPTOR.message_types_by_name['id'] = _ID
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

upRequest = _reflection.GeneratedProtocolMessageType('upRequest', (_message.Message,), {
  'DESCRIPTOR' : _UPREQUEST,
  '__module__' : 'file_server_pb2'
  # @@protoc_insertion_point(class_scope:upRequest)
  })
_sym_db.RegisterMessage(upRequest)

downRequest = _reflection.GeneratedProtocolMessageType('downRequest', (_message.Message,), {
  'DESCRIPTOR' : _DOWNREQUEST,
  '__module__' : 'file_server_pb2'
  # @@protoc_insertion_point(class_scope:downRequest)
  })
_sym_db.RegisterMessage(downRequest)

stringMes = _reflection.GeneratedProtocolMessageType('stringMes', (_message.Message,), {
  'DESCRIPTOR' : _STRINGMES,
  '__module__' : 'file_server_pb2'
  # @@protoc_insertion_point(class_scope:stringMes)
  })
_sym_db.RegisterMessage(stringMes)

bufferMes = _reflection.GeneratedProtocolMessageType('bufferMes', (_message.Message,), {
  'DESCRIPTOR' : _BUFFERMES,
  '__module__' : 'file_server_pb2'
  # @@protoc_insertion_point(class_scope:bufferMes)
  })
_sym_db.RegisterMessage(bufferMes)

fs_reply = _reflection.GeneratedProtocolMessageType('fs_reply', (_message.Message,), {
  'DESCRIPTOR' : _FS_REPLY,
  '__module__' : 'file_server_pb2'
  # @@protoc_insertion_point(class_scope:fs_reply)
  })
_sym_db.RegisterMessage(fs_reply)

id = _reflection.GeneratedProtocolMessageType('id', (_message.Message,), {
  'DESCRIPTOR' : _ID,
  '__module__' : 'file_server_pb2'
  # @@protoc_insertion_point(class_scope:id)
  })
_sym_db.RegisterMessage(id)



_FILE_SERVER = _descriptor.ServiceDescriptor(
  name='file_server',
  full_name='file_server',
  file=DESCRIPTOR,
  index=0,
  serialized_options=None,
  create_key=_descriptor._internal_create_key,
  serialized_start=281,
  serialized_end=523,
  methods=[
  _descriptor.MethodDescriptor(
    name='pwd',
    full_name='file_server.pwd',
    index=0,
    containing_service=None,
    input_type=_STRINGMES,
    output_type=_STRINGMES,
    serialized_options=None,
    create_key=_descriptor._internal_create_key,
  ),
  _descriptor.MethodDescriptor(
    name='ls',
    full_name='file_server.ls',
    index=1,
    containing_service=None,
    input_type=_ID,
    output_type=_STRINGMES,
    serialized_options=None,
    create_key=_descriptor._internal_create_key,
  ),
  _descriptor.MethodDescriptor(
    name='cd',
    full_name='file_server.cd',
    index=2,
    containing_service=None,
    input_type=_STRINGMES,
    output_type=_FS_REPLY,
    serialized_options=None,
    create_key=_descriptor._internal_create_key,
  ),
  _descriptor.MethodDescriptor(
    name='mkdir',
    full_name='file_server.mkdir',
    index=3,
    containing_service=None,
    input_type=_STRINGMES,
    output_type=_FS_REPLY,
    serialized_options=None,
    create_key=_descriptor._internal_create_key,
  ),
  _descriptor.MethodDescriptor(
    name='rm',
    full_name='file_server.rm',
    index=4,
    containing_service=None,
    input_type=_STRINGMES,
    output_type=_FS_REPLY,
    serialized_options=None,
    create_key=_descriptor._internal_create_key,
  ),
  _descriptor.MethodDescriptor(
    name='upload',
    full_name='file_server.upload',
    index=5,
    containing_service=None,
    input_type=_UPREQUEST,
    output_type=_FS_REPLY,
    serialized_options=None,
    create_key=_descriptor._internal_create_key,
  ),
  _descriptor.MethodDescriptor(
    name='download',
    full_name='file_server.download',
    index=6,
    containing_service=None,
    input_type=_DOWNREQUEST,
    output_type=_BUFFERMES,
    serialized_options=None,
    create_key=_descriptor._internal_create_key,
  ),
])
_sym_db.RegisterServiceDescriptor(_FILE_SERVER)

DESCRIPTOR.services_by_name['file_server'] = _FILE_SERVER

# @@protoc_insertion_point(module_scope)
