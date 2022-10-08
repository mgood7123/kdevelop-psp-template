cmake_minimum_required(VERSION 3.18)
project(psp-templates)

find_package(KDevPlatform REQUIRED)

set(example_template_DIRS
    template-psp-makefile
)

kdevplatform_add_app_templates(${example_template_DIRS})
