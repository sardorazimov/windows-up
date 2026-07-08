---
name: Bug report
title: "[Bug] "
description: Report a problem with the project
labels: bug
body:
  - type: textarea
    id: summary
    attributes:
      label: Summary
      description: Briefly describe the bug.
      placeholder: What happened?
    validations:
      required: true
  - type: textarea
    id: steps
    attributes:
      label: Steps to reproduce
      description: List the steps to reproduce the issue.
      placeholder: 1. Run ...
    validations:
      required: true
  - type: textarea
    id: expected
    attributes:
      label: Expected behavior
      description: What should happen instead?
    validations:
      required: true
  - type: textarea
    id: environment
    attributes:
      label: Environment
      description: OS, compiler, version, etc.
    validations:
      required: false
