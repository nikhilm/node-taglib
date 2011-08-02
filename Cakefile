fs = require 'fs'

run = (args, cb) ->
  proc =         require('child_process').spawn '/usr/bin/env', ["coffee"].concat args
  proc.stderr.on 'data', (buffer) -> console.log buffer.toString()
  proc.on        'exit', (status) ->
    process.exit(1) if status != 0
    cb() if typeof cb is 'function'

task 'build', 'build coffee script files', (cb) ->
  files = fs.readdirSync 'src'
  files = ('src/' + file for file in files when file.match(/\.coffee$/))
  run ['-c', '-o', 'lib'].concat(files), cb
