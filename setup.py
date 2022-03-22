from setuptools import setup, Extension


setup(name='mykmeanssp',
      version='1.0',
      description='calculates kmeans algo',
      ext_modules=[Extension('mykmeanssp', sources=['kmeans.c'])])

setup( name='files',
      version='1.0',
      description='prints observs',
      ext_modules=[Extension('files', sources=['observstofile.c'])])

