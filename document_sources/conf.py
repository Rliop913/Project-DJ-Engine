# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'Project_DJ_Engine'
copyright = '2025, RLIOP913'
author = 'RLIOP913'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration
import os
extensions = [
    'breathe', 
    'exhale',
    'sphinx.ext.graphviz' 
    ]

graphviz_output_format = 'svg'
breathe_projects = {
    "Project_DJ_Engine" : "./xml"
}

breathe_default_project = "Project_DJ_Engine"

exhale_args = {
    "containmentFolder" : "./api",
    "rootFileName": "api_root.rst",
    "rootFileTitle": "API Reference",
    "doxygenStripFromPath": "../include",
    "createTreeView": True,
    "exhaleExecutesDoxygen": False
}

templates_path = ['_templates']
exclude_patterns = []



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']
