import fs from 'fs'
import path from 'path'

export default function build(): string {
    let buildOutput = ''
    const indexHtmlContent = fs.readFileSync('./app/index.html', 'utf8')
    buildOutput += indexHtmlContent

    /*STYLES*/
    let styles = ''
    fs.readdirSync('./styles').forEach(file => {
        const filePath = path.join('./styles', file)
        console.log('adding',filePath)
        const fileContent = fs.readFileSync(filePath, 'utf8')
        styles += `<style>\n${fileContent}\n</style>\n`
    })
    buildOutput = buildOutput.replace('</head>', `${styles}\n${'</head>'}`)

    /*SCRITS*/
    let scripts = ''
    fs.readdirSync('./scripts').forEach(file => {
        const filePath = path.join('./scripts', file)
        console.log('adding',filePath)
        const fileContent = fs.readFileSync(filePath, 'utf8')
        scripts += `<script type="text/javascript">\n${fileContent}\n</script>\n`
    })
    buildOutput = buildOutput.replace('</head>', `${scripts}\n${'</head>'}`)

    /*TEMPLATES*/
    let templates = ''
    fs.readdirSync('./templates').forEach(file => {
        const filePath = path.join('./templates', file)
        console.log('adding',filePath)
        const templateId = path.parse(filePath).name
        const fileContent = fs.readFileSync(filePath, 'utf8')
        templates += `<script type="text/html" id="${templateId}">\n${fileContent}\n</script>\n`
    })
    buildOutput = buildOutput.replace('</head>', `${templates}\n${'</head>'}`)


    /*ICONS*/
    let icons = ''
    fs.readdirSync('./icons').forEach(file => {
        const filePath = path.join('./icons', file)
        console.log('adding',filePath)
        const templateId = `icon-${path.parse(filePath).name}` 
        const fileContent = fs.readFileSync(filePath, 'utf8')
        icons += `<script type="text/html" id="${templateId}">\n${fileContent}\n</script>\n`
    })
    buildOutput = buildOutput.replace('</head>', `${icons}\n${'</head>'}`)


    return buildOutput

}