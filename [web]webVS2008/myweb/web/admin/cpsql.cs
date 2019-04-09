namespace web.admin
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpsql : Page
    {
        protected Button btnedit;
        protected TextBox tbsql;

        private void btnedit_Click(object sender, EventArgs e)
        {
            new DataProviders().ExecuteSql(this.tbsql.Text.ToString());
        }

        private void InitializeComponent()
        {
            this.btnedit.Click += new EventHandler(this.btnedit_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            new WebLogic().isadmin();
            new system().loadConfig(0);
        }
    }
}
