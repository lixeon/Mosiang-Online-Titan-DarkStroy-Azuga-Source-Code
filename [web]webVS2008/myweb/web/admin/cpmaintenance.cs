namespace web.admin
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpmaintenance : Page
    {
        protected Button btnmaintenance;
        protected Button btnupdatemember;
        protected Button btnupdatememberbyalipay;

        private void btnmaintenance_Click(object sender, EventArgs e)
        {
            new DataProviders().ExecuteSql("EXEC Web_RebuildIndex");
        }

        private void btnupdatemember_Click(object sender, EventArgs e)
        {
            new DataProviders().ExecuteSql("EXEC Web_UpdateVIP 1");
        }

        private void btnupdatememberbyalipay_Click(object sender, EventArgs e)
        {
            new DataProviders().ExecuteSql("EXEC Web_UpdateVIP 2");
        }

        private void InitializeComponent()
        {
            this.btnmaintenance.Click += new EventHandler(this.btnmaintenance_Click);
            this.btnupdatemember.Click += new EventHandler(this.btnupdatemember_Click);
            this.btnupdatememberbyalipay.Click += new EventHandler(this.btnupdatememberbyalipay_Click);
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
        }
    }
}
