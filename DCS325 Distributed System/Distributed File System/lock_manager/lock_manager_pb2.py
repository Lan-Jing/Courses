# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: lock_manager.proto
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='lock_manager.proto',
  package='',
  syntax='proto3',
  serialized_options=None,
  create_key=_descriptor._internal_create_key,
  serialized_pb=b'\n\x12lock_manager.proto\".\n\x08\x66ileInfo\x12\x10\n\x08\x66ilePath\x18\x01 \x01(\t\x12\x10\n\x08lockType\x18\x02 \x01(\x05\"\x18\n\x08lm_reply\x12\x0c\n\x04\x64one\x18\x01 \x01(\x05\x32P\n\x0clock_manager\x12\x1e\n\x04lock\x12\t.fileInfo\x1a\t.lm_reply\"\x00\x12 \n\x06unlock\x12\t.fileInfo\x1a\t.lm_reply\"\x00\x62\x06proto3'
)




_FILEINFO = _descriptor.Descriptor(
  name='fileInfo',
  full_name='fileInfo',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='filePath', full_name='fileInfo.filePath', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='lockType', full_name='fileInfo.lockType', index=1,
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
  serialized_start=22,
  serialized_end=68,
)


_LM_REPLY = _descriptor.Descriptor(
  name='lm_reply',
  full_name='lm_reply',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='done', full_name='lm_reply.done', index=0,
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
  serialized_start=70,
  serialized_end=94,
)

DESCRIPTOR.message_types_by_name['fileInfo'] = _FILEINFO
DESCRIPTOR.message_types_by_name['lm_reply'] = _LM_REPLY
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

fileInfo = _reflection.GeneratedProtocolMessageType('fileInfo', (_message.Message,), {
  'DESCRIPTOR' : _FILEINFO,
  '__module__' : 'lock_manager_pb2'
  # @@protoc_insertion_point(class_scope:fileInfo)
  })
_sym_db.RegisterMessage(fileInfo)

lm_reply = _reflection.GeneratedProtocolMessageType('lm_reply', (_message.Message,), {
  'DESCRIPTOR' : _LM_REPLY,
  '__module__' : 'lock_manager_pb2'
  # @@protoc_insertion_point(class_scope:lm_reply)
  })
_sym_db.RegisterMessage(lm_reply)



_LOCK_MANAGER = _descriptor.ServiceDescriptor(
  name='lock_manager',
  full_name='lock_manager',
  file=DESCRIPTOR,
  index=0,
  serialized_options=None,
  create_key=_descriptor._internal_create_key,
  serialized_start=96,
  serialized_end=176,
  methods=[
  _descriptor.MethodDescriptor(
    name='lock',
    full_name='lock_manager.lock',
    index=0,
    containing_service=None,
    input_type=_FILEINFO,
    output_type=_LM_REPLY,
    serialized_options=None,
    create_key=_descriptor._internal_create_key,
  ),
  _descriptor.MethodDescriptor(
    name='unlock',
    full_name='lock_manager.unlock',
    index=1,
    containing_service=None,
    input_type=_FILEINFO,
    output_type=_LM_REPLY,
    serialized_options=None,
    create_key=_descriptor._internal_create_key,
  ),
])
_sym_db.RegisterServiceDescriptor(_LOCK_MANAGER)

DESCRIPTOR.services_by_name['lock_manager'] = _LOCK_MANAGER

# @@protoc_insertion_point(module_scope)
