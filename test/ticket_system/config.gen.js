#!/usr/bin/env node

const { readFileSync, writeFileSync } = require('fs')
const yaml = require('yaml')

const sec = 1000
const MiB = 1024 ** 2

const cfg = yaml.parse(readFileSync('config.in.yml').toString())
const range = ([ start, end ]) => Array.from(Array(end - start + 1).keys()).map(x => x + start)
const groups = Object.entries(cfg).map(([ name, value ], i) => ({
  GroupID: i + 1,
  GroupName: name,
  GroupScore: value.score,
  TestPoints: range(value.range),
}))
const testpoints = Object.entries(cfg).flatMap(([ name, value ]) => range(value.range).map((id, i) => ({
  ID: id,
  Dependency: i === 0 ? value.dep ? cfg[value.dep].range[1] : 0 : id - 1,
  TimeLimit: value.time * sec,
  MemoryLimit: value.mem * MiB,
  DiskLimit: (i === 0 ? -1 : 1) * value.disk * MiB,
  FileNumberLimit: 50,
  ValgrindTestOn: false,
})))
console.log(JSON.stringify({
  RunnerGroup: 'ticket',
  Groups: groups,
  Details: testpoints,
  CompileTimeLimit: 90 * sec,
  SPJ: 0,
}, null, 2))

