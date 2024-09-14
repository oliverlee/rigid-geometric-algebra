import os
import sys
import logging
import setuptools.dist
import tempfile
from pathlib import Path
from notebook.notebookapp import NotebookApp
from notebook.nbextensions import install_nbextension_python, ListNBExtensionsApp

logging.basicConfig(level = logging.INFO)
logger = logging.getLogger("notebook init")
logger.info("... starting notebook setup ...")

workspace = os.getenv("BUILD_WORKSPACE_DIRECTORY")
if not workspace:
    logger.error("""\
     Unable to determine workspace directory.

    This script is not intended to be run directly, use `bazel run`.
    """)
    sys.exit(1)

data_dir = tempfile.TemporaryDirectory()
os.environ["JUPYTER_DATA_DIR"] = data_dir.name

print(install_nbextension_python(
    "widgetsnbextension",
    overwrite = True,
    user=True,
    logger = logger,
))

ListNBExtensionsApp().list_nbextensions()

# https://github.com/jupyter/notebook/blob/v6.5.7/notebook/notebookapp.py
# https://jupyter-notebook.readthedocs.io/en/v6.5.4/examples/Notebook/Distributing%20Jupyter%20Extensions%20as%20Python%20Packages.html
# https://jupyter-notebook.readthedocs.io/en/v6.5.4/examples/Notebook/Distributing%20Jupyter%20Extensions%20as%20Python%20Packages.html#jupyter-config/jupyter_notebook_config.d/my_fancy_module.json
NotebookApp.launch_instance(
    #autoreload=True,
    file_to_run=(workspace + "/python/Untitled.ipynb"),
    log_level=logging.DEBUG,
)
