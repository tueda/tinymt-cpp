"""Configuration file for the Sphinx documentation builder."""

import os
import subprocess


try:
    import sphinx_rtd_theme

    has_sphinx_rtd_theme = True
except ImportError:
    has_sphinx_rtd_theme = False
    pass


# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------

project = "tinymt-cpp"
copyright = "2020, Takahiro Ueda"  # noqa: A001
author = "Takahiro Ueda"
version = "0.0.1"
release = version

# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = ["breathe", "m2r2"]

if has_sphinx_rtd_theme:
    extensions.append("sphinx_rtd_theme")

# Add any paths that contain templates here, relative to this directory.
templates_path = ["_templates"]

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ["_build", "Thumbs.db", ".DS_Store"]


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = "sphinx_rtd_theme" if has_sphinx_rtd_theme else "default"

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
# html_static_path = ["_static"]


# ----------------------------------------------------------------------------

# Breathe Configuration
breathe_default_project = "tinymt-cpp"
breathe_projects = {"tinymt-cpp": "xml"}
breathe_projects_source = {"tinymt-cpp": ("..", ["include/tinymt/tinymt.h"])}

# Check if we are running on Read the Docs' servers.
read_the_docs_build = os.environ.get("READTHEDOCS", None) == "True"

if read_the_docs_build:
    # Generate an XML input for Breathe.
    if not os.path.isdir("html_extra"):
        subprocess.run(["cmake", "-S", "..", "-B", "..", "-DBUILD_TESTING=OFF"])
        subprocess.run(["cmake", "--build", "..", "--target", "doc"])
        subprocess.run(["mkdir", "-p", "html_extra"])
        subprocess.run(["mv", "html", "html_extra/doxygen"])

html_extra_path = ["html_extra"]

# For local testing, one can run the following commands:
#
#  cmake -S .. -B ../build/docs -DBUILD_TESTING=OFF
#  cmake --build ../build/docs --target doc
#  rm -rf html_extra
#  mkdir html_extra
#  cp -rp ../build/docs/docs/xml xml
#  cp -rp ../build/docs/docs/html html_extra/doxygen
#  sphinx-build -M html . _build
