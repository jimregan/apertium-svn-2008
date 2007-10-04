<?php
	show_form("");

/*
  **************************
	   SHOW FORM
  **************************
*/
function show_form() {
print<<<_HTML_
<form class="translation" action="common/browser.php" method="post">
<table>
        <fieldset>
          <legend></legend>
          <tr>
                <label for="direccion3">
                <td>Translation:</td>
                <td>
                        <select id="direccion3"
                                name="direccion"
                                title="Select the translation type">
                                <option value="es-ca">Spanish-Catalan</option>
                                <option value="ca-es">Catalan-Spanish</option>
                                <option value="es-gl">Spanish-Galician</option>
                                <option value="gl-es">Galician-Spanish</option>
                                <option value="es-pt">Spanish-Portuguese(pt)</option>
                                <option value="pt-es">Portuguese-Spanish</option>
                                <option value="es-br">Spanish-Portuguese(br)</option>
                                <option value="oc-ca">Catalan-Aranese</option>
                                <option value="ca-oc">Aranese-Catalan</option>
                                <option value="fr-ca">Catalan-French</option>
                                <option value="ca-fr">French-Catalan</option>
                      <option value="ca-en">Catalan-English</option>
                      <option value="en-ca">English-Catalan</option>

                        </select>
</td>                        
                        </label></tr>
                        <tr>
                        <label for="inurl">
                        <td>URL:</td>
                        <td>                        
                        <input title="URL to be translated"
                               name="inurl"
                               type="text"
                               value="http://"/></td>
                               </label>
                               </tr>
                               <tr>
                <label for="marcar">
                <td>Mark unknown words:</td>
                <td>
                        <input name="marcar"
                               value="1"
                               type="checkbox"/>
                               </td>
                               </label>
                               </tr>

          </fieldset>
          <tr>
            <div>
                <td><input type="submit"
                       value="Translate"
                       class="submit"
                       title="Clic here to translate"/>

                <input type="reset"
                       value="Reset"
                       class="reset"
                       title="Clic here to reset"/></td>
           </div>
           </tr>
           </table>
      </form>
    </div>
_HTML_;
}