import fs from 'fs'
import build from './build'

const folderName = './disc';
const indexHtmlPath = './disc/index.html';

try {
    if (!fs.existsSync(folderName)) {
        fs.mkdirSync(folderName);
    }
    if (fs.existsSync(indexHtmlPath)) {
        fs.rmSync(indexHtmlPath);
    }

    const indexHtmlContent = build()
    fs.writeFileSync(indexHtmlPath, indexHtmlContent, 'utf8');

} catch (err) {
    console.error(err);
}