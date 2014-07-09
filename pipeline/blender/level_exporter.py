# python 3, export all objects to an arbitrary xml file
# format. This only works with Blender 2.6+

bl_info = {
    "name": "Export Bananaman XML",
    "author": "Utkarsh Sinha",
    "version": (1, 0),
    "blender": (2, 5, 7),
    "location": "File > Export > Bananaman Level XML",
    "description": "Export bananaman level (.xml)",
    "warning": "",
    "wiki_url": "",
    "tracker_url": "",
    "category": "Import-Export"}

import bpy
import bmesh

def _doexport(context, filepath):
    f = open(filepath, 'w', encoding='utf-8')
    f.write("<?xml version=\"1.0\"?>")
    f.write("<level>")
    f.write("</level>")
    f.write("<content>")
    f.write("</content>")
    f.close()
    return {'FINISHED'}


# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty
from bpy.types import Operator


class BananamanLevelXMLExport(Operator, ExportHelper):
    '''This appears in the tooltip of the operator and in the generated docs'''
    # important since its how bpy.ops.import_test.some_data is constructed
    bl_idname = "bananaman.levelxmlexport"
    bl_label = "Bananaman level XML"

    # ExportHelper mixin class uses this
    filename_ext = ".xml"

    filter_glob = StringProperty(
            default="*.xml",
            options={'HIDDEN'},
            )

    # List of operator properties, the attributes will be assigned
    # to the class instance from the operator settings before calling.
    # use_setting = BoolProperty(
    #         name="Example Boolean",
    #         description="Example Tooltip",
    #         default=True,
    #         )

    # type = EnumProperty(
    #         name="Example Enum",
    #         description="Choose between two items",
    #         items=(('OPT_A', "First Option", "Description one"),
    #                ('OPT_B', "Second Option", "Description two")),
    #         default='OPT_A',
    #         )

    def execute(self, context):
        return _doexport(context, self.filepath)


# Only needed if you want to add into a dynamic menu
def menu_func_export(self, context):
    self.layout.operator(BananamanLevelXMLExport.bl_idname, text="Bananaman Level XML")

def register():
    bpy.utils.register_class(BananamanLevelXMLExport)
    bpy.types.INFO_MT_file_export.append(menu_func_export)

def unregister():
    bpy.utils.unregister_class(BananamanLevelXMLExport)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)

if __name__ == "__main__":
    register()

    bpy.ops.bananaman.levelxmlexport('INVOKE_DEFAULT')
