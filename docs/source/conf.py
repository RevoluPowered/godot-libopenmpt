# Configuration file for the Sphinx documentation builder.

# -- Project information -----------------------------------------------------

project = 'libopenmpt-godot'
copyright = '2026, libopenmpt-godot Contributors'
author = 'libopenmpt-godot Contributors'
release = '1.0.0'

# -- General configuration ---------------------------------------------------

extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.viewcode',
    'sphinx.ext.napoleon',
    'sphinx.ext.intersphinx',
    'myst_parser',
]

templates_path = ['_templates']
exclude_patterns = []

# -- Options for HTML output -------------------------------------------------

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']

# -- Options for MyST parser -------------------------------------------------

myst_enable_extensions = [
    "colon_fence",
    "deflist",
]

# -- Intersphinx configuration -----------------------------------------------

intersphinx_mapping = {
    'godot': ('https://docs.godotengine.org/en/stable/', None),
}
