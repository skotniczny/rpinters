if build_standalone
  install_data('rpinters.ui', install_dir: ui_dir)
  install_data('10-rpinters.rules', install_dir: polkit_dir)
  i18n.merge_file(input: 'rpinters.desktop.in',
    output: 'rpinters.desktop',
    type: 'desktop',
    po_dir: '../po',
    install: true,
    install_dir: desktop_dir
  )
endif

if build_plugin
  install_data('rpinters.ui', install_dir: pui_dir)
  install_data('10-rpinters.rules', install_dir: polkit_dir)
endif
